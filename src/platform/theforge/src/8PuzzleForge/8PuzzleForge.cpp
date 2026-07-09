// 8PuzzleForge — degrau 3 da PoC The-Forge (.ai/task/01-theforge-poc.md):
// o 8Puzzle completo rodando na plataforma The-Forge.
//
// Mesma fiacao do main_ftxui.cpp (dominio, servicos, GameRouter e maquina de
// estados intocados; recordes no mesmo records.tsv), trocando a PLATAFORMA:
// o IApp do The-Forge hospeda o EngineManager da cengine em MODO HOSPEDADO
// (cengine 0.4.0, task 15): sem window manager (a janela e do host) e sem
// start() — o host dirige o quadro:
//
//   Update(dt) -> beginInput + guarda o dt
//   Draw()     -> beginDraw + engine.frame(dt); false -> requestShutdown()
//
// O frame(dt) executa as fases (onEnter -> input -> update(fixedDt) 0..N
// vezes -> render -> onExit) com o acumulador de fixed timestep DENTRO da
// engine — o jogo volta a receber dt constante, como nas plataformas
// terminal/ftxui. As cenas ficam em scene/ e falam com a plataforma so
// atraves do forgeui (fila de teclado + desenho de texto) — ver ForgeUi.h.

// cengine + jogo — C++ puro, ANTES dos headers do The-Forge (IMemory.h por ultimo).
#include <cengine/core/EngineManager.hpp>
#include <cengine/core/Time.hpp>
#include <cengine/routing/GameManager.hpp>
#include <cengine/routing/RouterInMemory.hpp>
#include <cengine/routing/SceneRepository.hpp>

#include "8puzzle/game/GameRouter.h"
#include "8puzzle/game/service/GamePlayService.h"
#include "8puzzle/game/service/RecordService.h"
#include "8puzzle/game/service/repository/FileRecordRepository.h"
#include "8puzzle/game/service/repository/GamePlayRepository.h"
#include "8puzzle/game/state/StateGame.h"

#include "ForgeSceneFactory.h"
#include "ForgeUi.h"

#include <memory>

// Interfaces
#include "Common_3/Application/Interfaces/IApp.h"
#include "Common_3/Application/Interfaces/IFont.h"
#include "Common_3/Application/Interfaces/IUI.h"
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

std::shared_ptr<cengine::routing::RouterInMemory> gRouter;
std::unique_ptr<cengine::core::EngineManager>     gEngine;

// dt do Update() do host, consumido pelo frame() no Draw() (receita do modo
// hospedado — ver task 15 da cengine).
float gDt = 0.0f;

class PuzzleForge: public IApp
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

        UserInterfaceDesc uiRenderDesc = {};
        uiRenderDesc.pRenderer = pRenderer;
        initUserInterface(&uiRenderDesc);

        forgeui::initBindings();

        // fiacao do jogo (identica ao main_ftxui.cpp)
        auto sceneRepository = std::make_unique<cengine::routing::SceneRepository>();
        cengine::routing::ISceneRepository& sceneRepositoryRef = *sceneRepository;

        gRouter = std::make_shared<cengine::routing::RouterInMemory>(
            std::move(sceneRepository), std::make_unique<InitialSG>());

        const auto gameRouter = std::make_shared<GameRouter>(gRouter);

        const auto gamePlayRepository = std::make_shared<GamePlayRepository>();
        const auto gamePlayService = std::make_shared<GamePlayService>(gamePlayRepository);
        // Recordes persistidos no mesmo arquivo das outras plataformas
        // (relativo ao diretorio de trabalho — o OutDir do exe).
        const auto recordRepository = std::make_shared<FileRecordRepository>("records.tsv");
        const auto recordService = std::make_shared<RecordService>(recordRepository);

        ForgeSceneFactory::populateForgeScenes(sceneRepositoryRef, gameRouter, gamePlayService, recordService);

        // Modo hospedado: sem window manager (janela e do The-Forge) e sem
        // start() — o Draw() dirige via frame(dt).
        gEngine = std::make_unique<cengine::core::EngineManager>(
            nullptr, std::make_unique<cengine::routing::GameManager>(gRouter));

        waitForAllResourceLoads();

        return true;
    }

    void Exit()
    {
        gEngine->cleanup(); // no modo hospedado o cleanup e do host
        gEngine.reset();
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
        // O quadro inteiro roda no frame() do Draw(); aqui so capturamos o
        // input do host e guardamos o dt que ele mediu.
        forgeui::beginInput(!uiIsFocused());
        gDt = deltaTime;
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

        // a cena atual desenha atraves do alvo publicado no forgeui; o
        // frame() executa o quadro completo da cengine (fases + fixed
        // timestep) e devolve false quando o jogo pediu saida.
        forgeui::beginDraw(cmd, (float)pRenderTarget->mWidth, (float)pRenderTarget->mHeight, gFontID);
        const bool keepRunning = gEngine->frame(cengine::core::Seconds{ (double)gDt });

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

        if (!keepRunning)
        {
            requestShutdown();
        }
    }

    const char* GetName() { return "8PuzzleForge"; }

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

DEFINE_APPLICATION_MAIN(PuzzleForge)
