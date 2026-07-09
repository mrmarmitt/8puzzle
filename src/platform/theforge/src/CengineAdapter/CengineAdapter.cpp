// CengineAdapter — degrau 2 da PoC The-Forge (.ai/task/01-theforge-poc.md).
//
// O IApp hospeda o GameManager da cengine: o The-Forge e dono do loop/janela/
// input (fase 1), e cada callback do framework e mapeado para as fases do
// cengine::routing::GameManager:
//
//   Update(dt) -> onEnter() + input() + update(Seconds{dt})
//   Draw()     -> render() + onExit(); shouldExit() -> requestShutdown()
//
// Quem decide cenas/estados continua sendo a cengine (router + repositorio);
// o loop do The-Forge vira detalhe mecanico da plataforma. As fontes da
// cengine (routing) sao compiladas direto neste vcxproj a partir do repo
// irmao ../cengine (mesma versao da tag 0.3.0 usada pelo build CMake).
//
// Cenas de teste: A <-> B (ENTER navega, provando o ciclo onEnter/onExit e a
// reativacao A->B->A com recriacao da cena) e exit (ESC), que encerra o app.

// cengine — C++ puro, ANTES dos headers do The-Forge (IMemory.h por ultimo).
#include <cengine/core/IScene.hpp>
#include <cengine/core/Time.hpp>
#include <cengine/routing/GameManager.hpp>
#include <cengine/routing/IRouter.hpp>
#include <cengine/routing/IState.hpp>
#include <cengine/routing/RouterInMemory.hpp>
#include <cengine/routing/SceneRepository.hpp>
#include <cengine/routing/StateCodes.hpp>

#include <memory>
#include <string>

// Interfaces
#include "Common_3/Application/Interfaces/IApp.h"
#include "Common_3/Application/Interfaces/IFont.h"
#include "Common_3/Application/Interfaces/IUI.h"
#include "Common_3/OS/Interfaces/IInput.h"
#include "Common_3/Utilities/Interfaces/IFileSystem.h"
#include "Common_3/Utilities/Interfaces/ILog.h"

#include "Common_3/Utilities/RingBuffer.h"

// Renderer
#include "Common_3/Graphics/Interfaces/IGraphics.h"
#include "Common_3/Resources/ResourceLoader/Interfaces/IResourceLoader.h"

// fsl (INIT_RS_DESC)
#include "Common_3/Graphics/FSL/defaults.h"

#include "Common_3/Utilities/Interfaces/IMemory.h" // deve ser o ultimo include

// Duas cenas de recursos: uma em voo na GPU, uma sendo gravada na CPU.
const uint32_t gDataBufferCount = 2;

Renderer*  pRenderer = NULL;
Queue*     pGraphicsQueue = NULL;
GpuCmdRing gGraphicsCmdRing = {};

SwapChain* pSwapChain = NULL;
Semaphore* pImageAcquiredSemaphore = NULL;

uint32_t gFrameIndex = 0;
uint32_t gFontID = 0;

// Bindings de navegacao das cenas de teste (resolvidos no Init).
InputEnum gNavNext = {};
InputEnum gNavExit = {};

/// Ponte quadro-a-quadro entre o IApp e as cenas: o app preenche o snapshot
/// de input no Update e o alvo de desenho no Draw; as cenas so leem daqui.
/// (No degrau 3 isto evolui para um contexto injetado via factory, como o
/// FtxuiWindowManager faz — global e suficiente para a PoC.)
struct ForgeFrame
{
    Cmd*  cmd = NULL;
    float width = 0.0f;
    float height = 0.0f;
    bool  nextPressed = false;
    bool  exitPressed = false;
};
ForgeFrame gFrame = {};

std::shared_ptr<cengine::routing::IRouter>    gRouter;
std::unique_ptr<cengine::routing::GameManager> gGameManager;

static void drawTextCentered(const char* text, float y, float size, uint32_t abgr)
{
    FontDrawDesc desc = {};
    desc.pText = text;
    desc.mFontID = gFontID;
    desc.mFontColor = abgr;
    desc.mFontSize = size;
    const float2 measured = fntMeasureFontText(text, &desc);
    cmdDrawTextWithFont(gFrame.cmd, float2((gFrame.width - measured.x) * 0.5f, y), &desc);
}

/// Estado generico da PoC: codigo + nome legivel.
class AppState final: public cengine::routing::IState
{
    std::string m_code;
    std::string m_name;

public:
    AppState(std::string code, std::string name): m_code(std::move(code)), m_name(std::move(name)) {}

    [[nodiscard]] std::string getCode() const override { return m_code; }
    [[nodiscard]] std::string getName() const override { return m_name; }
};

/// Cena de teste: mostra o proprio nome, o tempo/updates acumulados (prova o
/// update(dt) e que a recriacao no reload zera o estado) e navega com
/// ENTER (proxima) / ESC (sair).
class DemoScene final: public cengine::core::IScene
{
    const char* m_label;
    const char* m_nextCode;
    const char* m_nextLabel;
    uint32_t    m_accent; // ABGR

    double   m_elapsed = 0.0;
    uint64_t m_updates = 0;

public:
    DemoScene(const char* label, const char* nextCode, const char* nextLabel, uint32_t accent):
        m_label(label), m_nextCode(nextCode), m_nextLabel(nextLabel), m_accent(accent)
    {
    }

    void onEnter() override { LOGF(eINFO, "[cengine] onEnter: %s", m_label); }

    void update(cengine::core::Seconds dt) override
    {
        m_elapsed += dt.count();
        ++m_updates;
    }

    void input() override
    {
        if (gFrame.nextPressed)
        {
            gRouter->requestState(std::make_unique<AppState>(m_nextCode, m_nextLabel));
        }
        else if (gFrame.exitPressed)
        {
            gRouter->requestState(std::make_unique<AppState>(std::string(cengine::routing::kExitStateCode), "Exit"));
        }
    }

    void draw() override
    {
        drawTextCentered(m_label, gFrame.height * 0.26f, 64.0f, m_accent);

        char status[128];
        snprintf(status, sizeof(status), "tempo na cena: %.1fs  |  updates: %llu", m_elapsed, (unsigned long long)m_updates);
        drawTextCentered(status, gFrame.height * 0.26f + 96.0f, 22.0f, 0xffffffff);

        char nav[128];
        snprintf(nav, sizeof(nav), "ENTER -> %s   |   ESC -> sair", m_nextLabel);
        drawTextCentered(nav, gFrame.height * 0.26f + 136.0f, 22.0f, 0xffaaaaaa);

        drawTextCentered("cengine GameManager hospedado no IApp — degrau 2 da PoC", gFrame.height * 0.88f, 18.0f, 0xff888888);
    }

    void onExit() override { LOGF(eINFO, "[cengine] onExit: %s (%.1fs, %llu updates)", m_label, m_elapsed, (unsigned long long)m_updates); }
};

/// Cena do estado de saida: nunca chega a desenhar — o adaptador ve
/// shouldExit() logo apos o commit e pede o shutdown do framework.
class ExitScene final: public cengine::core::IScene
{
public:
    void onEnter() override {}
    void update(cengine::core::Seconds) override {}
    void draw() override {}
    void input() override {}
    void onExit() override {}
};

class CengineAdapter: public IApp
{
public:
    bool Init()
    {
        // renderer
        RendererDesc settings;
        memset(&settings, 0, sizeof(settings));
        initGPUConfiguration(settings.pExtendedSettings);
        initRenderer(GetName(), &settings, &pRenderer);
        if (!pRenderer)
        {
            ShowUnsupportedMessage(getUnsupportedGPUMsg());
            return false;
        }
        setupGPUConfigurationPlatformParameters(pRenderer, settings.pExtendedSettings);

        QueueDesc queueDesc = {};
        queueDesc.mType = QUEUE_TYPE_GRAPHICS;
        queueDesc.mFlag = QUEUE_FLAG_INIT_MICROPROFILE;
        initQueue(pRenderer, &queueDesc, &pGraphicsQueue);

        GpuCmdRingDesc cmdRingDesc = {};
        cmdRingDesc.pQueue = pGraphicsQueue;
        cmdRingDesc.mPoolCount = gDataBufferCount;
        cmdRingDesc.mCmdPerPoolCount = 1;
        cmdRingDesc.mAddSyncPrimitives = true;
        initGpuCmdRing(pRenderer, &cmdRingDesc, &gGraphicsCmdRing);

        initSemaphore(pRenderer, &pImageAcquiredSemaphore);

        initResourceLoaderInterface(pRenderer);

        RootSignatureDesc rootDesc = {};
        INIT_RS_DESC(rootDesc, "default.rootsig", "compute.rootsig");
        initRootSignature(pRenderer, &rootDesc);

        // fontes (mesma fonte dos unit tests; resolvida via PathStatement.txt)
        FontDesc font = {};
        font.pFontPath = "TitilliumText/TitilliumText-Bold.otf";
        fntDefineFonts(&font, 1, &gFontID);

        FontSystemDesc fontRenderDesc = {};
        fontRenderDesc.pRenderer = pRenderer;
        if (!initFontSystem(&fontRenderDesc))
            return false;

        // UI middleware (base das cenas do 8Puzzle no degrau 3)
        UserInterfaceDesc uiRenderDesc = {};
        uiRenderDesc.pRenderer = pRenderer;
        initUserInterface(&uiRenderDesc);

        // input: ENTER/SPACE avancam de cena; "exit" (ESC released) e builtin.
        inputAddCustomBindings("scene_next; button; K_ENTER; released\n"
                               "scene_next; button; K_SPACE; released");
        gNavNext = inputGetCustomBindingEnum("scene_next");
        gNavExit = inputGetCustomBindingEnum("exit");

        // montagem cengine: repositorio (factories) -> router -> GameManager.
        // As factories rodam so no primeiro getScene de cada estado — gRouter
        // ja esta atribuido nesse ponto.
        auto repository = std::make_unique<cengine::routing::SceneRepository>();
        repository->registerFactory("scene_a",
                                    [] { return std::make_unique<DemoScene>("CENA A", "scene_b", "Cena B", 0xffffb300u); });
        repository->registerFactory("scene_b",
                                    [] { return std::make_unique<DemoScene>("CENA B", "scene_a", "Cena A", 0xff00b3ffu); });
        repository->registerFactory(std::string(cengine::routing::kExitStateCode), [] { return std::make_unique<ExitScene>(); });

        gRouter = std::make_shared<cengine::routing::RouterInMemory>(std::move(repository),
                                                                     std::make_unique<AppState>("scene_a", "Cena A"));
        gGameManager = std::make_unique<cengine::routing::GameManager>(gRouter);

        waitForAllResourceLoads();

        return true;
    }

    void Exit()
    {
        gGameManager->cleanup();
        gGameManager.reset();
        gRouter.reset();

        exitUserInterface();

        exitFontSystem();

        exitGpuCmdRing(pRenderer, &gGraphicsCmdRing);
        exitSemaphore(pRenderer, pImageAcquiredSemaphore);

        exitRootSignature(pRenderer);
        exitResourceLoaderInterface(pRenderer);

        exitQueue(pRenderer, pGraphicsQueue);

        exitRenderer(pRenderer);
        exitGPUConfiguration();
        pRenderer = NULL;
    }

    bool Load(ReloadDesc* pReloadDesc)
    {
        if (pReloadDesc->mType & (RELOAD_TYPE_RESIZE | RELOAD_TYPE_RENDERTARGET))
        {
            if (!addSwapChain())
                return false;
        }

        UserInterfaceLoadDesc uiLoad = {};
        uiLoad.mColorFormat = pSwapChain->ppRenderTargets[0]->mFormat;
        uiLoad.mHeight = mSettings.mHeight;
        uiLoad.mWidth = mSettings.mWidth;
        uiLoad.mLoadType = pReloadDesc->mType;
        loadUserInterface(&uiLoad);

        FontSystemLoadDesc fontLoad = {};
        fontLoad.mColorFormat = pSwapChain->ppRenderTargets[0]->mFormat;
        fontLoad.mHeight = mSettings.mHeight;
        fontLoad.mWidth = mSettings.mWidth;
        fontLoad.mLoadType = pReloadDesc->mType;
        loadFontSystem(&fontLoad);

        return true;
    }

    void Unload(ReloadDesc* pReloadDesc)
    {
        waitQueueIdle(pGraphicsQueue);

        unloadFontSystem(pReloadDesc->mType);
        unloadUserInterface(pReloadDesc->mType);

        if (pReloadDesc->mType & (RELOAD_TYPE_RESIZE | RELOAD_TYPE_RENDERTARGET))
        {
            removeSwapChain(pRenderer, pSwapChain);
        }
    }

    void Update(float deltaTime)
    {
        // snapshot de input do quadro (as cenas leem de gFrame no input())
        const bool uiFocused = uiIsFocused();
        gFrame.nextPressed = !uiFocused && inputGetValue(0, gNavNext) > 0.0f;
        gFrame.exitPressed = !uiFocused && inputGetValue(0, gNavExit) > 0.0f;

        // fases da cengine dirigidas pelo loop do The-Forge. dt aqui e
        // variavel (com clamp do framework) — formalizar passo fixo no modo
        // hospedado e a task 15 da cengine (fase 2).
        gGameManager->onEnter();
        gGameManager->input();
        gGameManager->update(cengine::core::Seconds{ (double)deltaTime });
    }

    void Draw()
    {
        if ((bool)pSwapChain->mEnableVsync != mSettings.mVSyncEnabled)
        {
            waitQueueIdle(pGraphicsQueue);
            ::toggleVSync(pRenderer, &pSwapChain);
        }

        uint32_t swapchainImageIndex;
        acquireNextImage(pRenderer, pSwapChain, pImageAcquiredSemaphore, NULL, &swapchainImageIndex);

        RenderTarget*     pRenderTarget = pSwapChain->ppRenderTargets[swapchainImageIndex];
        GpuCmdRingElement elem = getNextGpuCmdRingElement(&gGraphicsCmdRing, true, 1);

        // Espera se a CPU estiver gDataBufferCount quadros a frente da GPU.
        FenceStatus fenceStatus;
        getFenceStatus(pRenderer, elem.pFence, &fenceStatus);
        if (fenceStatus == FENCE_STATUS_INCOMPLETE)
            waitForFences(pRenderer, 1, &elem.pFence);

        resetCmdPool(pRenderer, elem.pCmdPool);

        Cmd* cmd = elem.pCmds[0];
        beginCmd(cmd);

        RenderTargetBarrier barriers[] = {
            { pRenderTarget, RESOURCE_STATE_PRESENT, RESOURCE_STATE_RENDER_TARGET },
        };
        cmdResourceBarrier(cmd, 0, NULL, 0, NULL, 1, barriers);

        BindRenderTargetsDesc bindRenderTargets = {};
        bindRenderTargets.mRenderTargetCount = 1;
        bindRenderTargets.mRenderTargets[0] = { pRenderTarget, LOAD_ACTION_CLEAR };
        bindRenderTargets.mDepthStencil = { NULL, LOAD_ACTION_DONTCARE };
        cmdBindRenderTargets(cmd, &bindRenderTargets);
        cmdSetViewport(cmd, 0.0f, 0.0f, (float)pRenderTarget->mWidth, (float)pRenderTarget->mHeight, 0.0f, 1.0f);
        cmdSetScissor(cmd, 0, 0, pRenderTarget->mWidth, pRenderTarget->mHeight);

        // a cena atual desenha atraves do alvo publicado em gFrame
        gFrame.cmd = cmd;
        gFrame.width = (float)pRenderTarget->mWidth;
        gFrame.height = (float)pRenderTarget->mHeight;
        gGameManager->render();

        cmdDrawUserInterface(cmd);

        cmdBindRenderTargets(cmd, NULL);

        barriers[0] = { pRenderTarget, RESOURCE_STATE_RENDER_TARGET, RESOURCE_STATE_PRESENT };
        cmdResourceBarrier(cmd, 0, NULL, 0, NULL, 1, barriers);

        endCmd(cmd);

        FlushResourceUpdateDesc flushUpdateDesc = {};
        flushUpdateDesc.mNodeIndex = 0;
        flushResourceUpdates(&flushUpdateDesc);
        Semaphore* waitSemaphores[2] = { flushUpdateDesc.pOutSubmittedSemaphore, pImageAcquiredSemaphore };

        QueueSubmitDesc submitDesc = {};
        submitDesc.mCmdCount = 1;
        submitDesc.mSignalSemaphoreCount = 1;
        submitDesc.mWaitSemaphoreCount = TF_ARRAY_COUNT(waitSemaphores);
        submitDesc.ppCmds = &cmd;
        submitDesc.ppSignalSemaphores = &elem.pSemaphore;
        submitDesc.ppWaitSemaphores = waitSemaphores;
        submitDesc.pSignalFence = elem.pFence;
        queueSubmit(pGraphicsQueue, &submitDesc);

        QueuePresentDesc presentDesc = {};
        presentDesc.mIndex = (uint8_t)swapchainImageIndex;
        presentDesc.mWaitSemaphoreCount = 1;
        presentDesc.pSwapChain = pSwapChain;
        presentDesc.ppWaitSemaphores = &elem.pSemaphore;
        presentDesc.mSubmitDone = true;

        queuePresent(pGraphicsQueue, &presentDesc);

        gFrameIndex = (gFrameIndex + 1) % gDataBufferCount;

        // fim da iteracao: efetiva navegacao pendente e encerra se o jogo
        // chegou ao estado de saida (mapeamento shouldExit -> framework).
        gGameManager->onExit();
        if (gGameManager->shouldExit())
        {
            requestShutdown();
        }
    }

    const char* GetName() { return "CengineAdapter"; }

    bool addSwapChain()
    {
        SwapChainDesc swapChainDesc = {};
        swapChainDesc.mWindowHandle = pWindow->handle;
        swapChainDesc.mPresentQueueCount = 1;
        swapChainDesc.ppPresentQueues = &pGraphicsQueue;
        swapChainDesc.mWidth = mSettings.mWidth;
        swapChainDesc.mHeight = mSettings.mHeight;
        swapChainDesc.mImageCount = getRecommendedSwapchainImageCount(pRenderer, &pWindow->handle);
        swapChainDesc.mColorFormat = getSupportedSwapchainFormat(pRenderer, &swapChainDesc, COLOR_SPACE_SDR_SRGB);
        swapChainDesc.mColorSpace = COLOR_SPACE_SDR_SRGB;
        swapChainDesc.mColorClearValue = { { 0.04f, 0.07f, 0.12f, 1.0f } }; // azul-noite
        swapChainDesc.mEnableVsync = mSettings.mVSyncEnabled;
        ::addSwapChain(pRenderer, &swapChainDesc, &pSwapChain);

        return pSwapChain != NULL;
    }
};

DEFINE_APPLICATION_MAIN(CengineAdapter)
