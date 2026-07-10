// ForgeLibSpike — degraus 0 e 1 da fase 2 (.ai/task/02-theforge-fase2-library-mode.md).
//
// The-Forge SEM IApp, SEM DEFINE_APPLICATION_MAIN e SEM WindowsMain:
// - degrau 0: janela Win32 propria + initRenderer + addSwapChain(HWND) +
//   clear azul-noite (validado em 2026-07-09);
// - degrau 1 (este arquivo): sistema de FONTES (fontstash) e INPUT proprio —
//   WM_KEYDOWN/WM_CHAR do nosso WndProc alimentam a fila do forgeui, a mesma
//   API que as cenas do 8PuzzleForge consomem no modo hospedado.
//
// Dependencia oculta exposta pelo degrau 1: o fontstash e criado por
// platformInitFontSystem() (chamado pelo WindowsMain em modo hospedado), que
// le o DPI do monitor via window system do framework (getActiveMonitorIdx usa
// o global gWindow + gMonitors). A cola minima replica o que o WindowsBase
// faz: initWindowSystem() (enumera monitores) + apontar gWindow para um
// WindowDesc nosso com o HWND. Sao simbolos extern do OS.lib nao expostos
// nos headers de interface — mesma tecnica do proprio WindowsBase.cpp.
//
// Ainda FORA do spike: resize (janela fixa), cengine (degrau 2), jogo (3).

#include "Common_3/Application/Interfaces/IFont.h"
#include "Common_3/Graphics/Interfaces/IGraphics.h"
#include "Common_3/OS/Interfaces/IOperatingSystem.h"
#include "Common_3/Resources/ResourceLoader/Interfaces/IResourceLoader.h"
#include "Common_3/Utilities/Interfaces/IFileSystem.h"
#include "Common_3/Utilities/Interfaces/ILog.h"

#include "Common_3/Utilities/RingBuffer.h"

// fsl (INIT_RS_DESC)
#include "Common_3/Graphics/FSL/defaults.h"

// fila de teclado + desenho de texto — mesma API das cenas do 8PuzzleForge
#include "8PuzzleForge/ForgeUi.h"

#include <string>

#include "Common_3/Utilities/Interfaces/IMemory.h" // deve ser o ultimo include

// O DEFINE_APPLICATION_MAIN exporta estes simbolos para ativar o Agility SDK
// do D3D12 (D3D12Core.dll copiada para a pasta do exe). Sem IApp, exportamos
// por conta propria — D3D12_AGILITY_SDK_VERSION vem do TF_Shared.props.
extern "C"
{
    __declspec(dllexport) extern const UINT D3D12SDKVersion = D3D12_AGILITY_SDK_VERSION;
    __declspec(dllexport) extern const char* D3D12SDKPath = "";
}

// Cola minima do sistema de fontes (ver comentario no topo): simbolos do
// OS.lib fora dos headers de interface, declarados como o WindowsBase faz.
extern bool        initWindowSystem();     // WindowsWindow.cpp: classe + monitores
extern void        exitWindowSystem();     // WindowsWindow.cpp: libera monitores
extern WindowDesc* gWindow;                // WindowsWindow.cpp: usado por getActiveMonitorIdx
extern bool        platformInitFontSystem(); // FontSystem.cpp: cria o contexto fontstash
extern void        platformExitFontSystem();  // FontSystem.cpp

namespace {

constexpr const char* kAppName = "ForgeLibSpike";
constexpr int32_t     kWidth = 1280;
constexpr int32_t     kHeight = 720;
constexpr uint32_t    gDataBufferCount = 2;

Renderer*  pRenderer = NULL;
Queue*     pGraphicsQueue = NULL;
GpuCmdRing gGraphicsCmdRing = {};
SwapChain* pSwapChain = NULL;
Semaphore* pImageAcquiredSemaphore = NULL;

uint32_t gFontID = 0;

WindowDesc gWindowDesc = {}; // alvo do global gWindow (cola do fontstash)

bool gQuit = false;

// estado da demo de input (alimentado pela fila do forgeui, 1 evento/quadro)
std::string gTyped;
std::string gLastKey = "-";
uint32_t    gArrowCount = 0;

// ---------------------------------------------------------------- janela

Key vkToKey(const WPARAM vk)
{
    switch (vk)
    {
    case VK_UP:     return Key::Up;
    case VK_DOWN:   return Key::Down;
    case VK_LEFT:   return Key::Left;
    case VK_RIGHT:  return Key::Right;
    case VK_RETURN: return Key::Enter;
    case VK_ESCAPE: return Key::Escape;
    case VK_BACK:   return Key::Backspace;
    default:        return Key::None;
    }
}

LRESULT CALLBACK spikeWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_KEYDOWN:
    {
        // teclas de controle entram na fila do forgeui; ESC tambem fecha o
        // spike (no jogo de verdade quem decide sair e a cena/router)
        const Key key = vkToKey(wParam);
        if (key != Key::None)
        {
            forgeui::pushKey({ key, '\0' });
        }
        if (wParam == VK_ESCAPE)
        {
            PostMessageW(hwnd, WM_CLOSE, 0, 0);
        }
        return 0;
    }
    case WM_CHAR:
        // caracteres imprimiveis (o WM_CHAR ja aplica shift/layout do teclado)
        if (wParam >= 32 && wParam < 127)
        {
            forgeui::pushKey({ Key::Char, (char)wParam });
        }
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        break;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

HWND createSpikeWindow()
{
    const HINSTANCE instance = GetModuleHandleW(NULL);

    WNDCLASSEXW wc = {};
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = spikeWndProc;
    wc.hInstance = instance;
    wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.lpszClassName = L"ForgeLibSpikeClass";
    if (!RegisterClassExW(&wc))
    {
        return NULL;
    }

    // Janela de tamanho FIXO (sem thick frame/maximize): resize e escopo do
    // degrau 2 — aqui o swapchain nunca precisa ser recriado.
    const DWORD style = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);
    RECT rect = { 0, 0, kWidth, kHeight };
    AdjustWindowRect(&rect, style, FALSE);

    return CreateWindowExW(0, wc.lpszClassName, L"Forge Lib Spike — degrau 1 (ESC sai)", style,
                           CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
                           NULL, NULL, instance, NULL);
}

void pumpMessages()
{
    MSG msg;
    while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            gQuit = true;
        }
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

// ---------------------------------------------------------------- graficos

bool initGraphics(HWND hwnd)
{
    RendererDesc settings;
    memset(&settings, 0, sizeof(settings));
    initGPUConfiguration(settings.pExtendedSettings);
    initRenderer(kAppName, &settings, &pRenderer);
    if (!pRenderer)
    {
        LOGF(eERROR, "[spike] GPU sem suporte: %s", getUnsupportedGPUMsg());
        return false;
    }
    setupGPUConfigurationPlatformParameters(pRenderer, settings.pExtendedSettings);

    QueueDesc queueDesc = {};
    queueDesc.mType = QUEUE_TYPE_GRAPHICS;
    initQueue(pRenderer, &queueDesc, &pGraphicsQueue);

    GpuCmdRingDesc cmdRingDesc = {};
    cmdRingDesc.pQueue = pGraphicsQueue;
    cmdRingDesc.mPoolCount = gDataBufferCount;
    cmdRingDesc.mCmdPerPoolCount = 1;
    cmdRingDesc.mAddSyncPrimitives = true;
    initGpuCmdRing(pRenderer, &cmdRingDesc, &gGraphicsCmdRing);

    initSemaphore(pRenderer, &pImageAcquiredSemaphore);

    initResourceLoaderInterface(pRenderer);

    // rootsigs por-app compilados pelo passo FSL (mesma receita da fase 1)
    RootSignatureDesc rootDesc = {};
    INIT_RS_DESC(rootDesc, "default.rootsig", "compute.rootsig");
    initRootSignature(pRenderer, &rootDesc);

    // fontes: o contexto fontstash ja existe (platformInitFontSystem na
    // main); aqui entram a fonte e os recursos de GPU do sistema
    FontDesc font = {};
    font.pFontPath = "TitilliumText/TitilliumText-Bold.otf";
    fntDefineFonts(&font, 1, &gFontID);

    FontSystemDesc fontRenderDesc = {};
    fontRenderDesc.pRenderer = pRenderer;
    if (!initFontSystem(&fontRenderDesc))
    {
        LOGF(eERROR, "[spike] initFontSystem falhou");
        return false;
    }

    // O swapchain recebe o HWND da NOSSA janela — este e o ponto da prova.
    WindowHandle handle = {};
    handle.type = WINDOW_HANDLE_TYPE_WIN32;
    handle.window = hwnd;

    SwapChainDesc swapChainDesc = {};
    swapChainDesc.mWindowHandle = handle;
    swapChainDesc.mPresentQueueCount = 1;
    swapChainDesc.ppPresentQueues = &pGraphicsQueue;
    swapChainDesc.mWidth = (uint32_t)kWidth;
    swapChainDesc.mHeight = (uint32_t)kHeight;
    swapChainDesc.mImageCount = getRecommendedSwapchainImageCount(pRenderer, &handle);
    swapChainDesc.mColorFormat = getSupportedSwapchainFormat(pRenderer, &swapChainDesc, COLOR_SPACE_SDR_SRGB);
    swapChainDesc.mColorSpace = COLOR_SPACE_SDR_SRGB;
    swapChainDesc.mColorClearValue = { { 0.04f, 0.07f, 0.12f, 1.0f } }; // azul-noite
    swapChainDesc.mEnableVsync = true;
    addSwapChain(pRenderer, &swapChainDesc, &pSwapChain);
    if (!pSwapChain)
    {
        return false;
    }

    FontSystemLoadDesc fontLoad = {};
    fontLoad.mColorFormat = pSwapChain->ppRenderTargets[0]->mFormat;
    fontLoad.mWidth = (uint32_t)kWidth;
    fontLoad.mHeight = (uint32_t)kHeight;
    fontLoad.mLoadType = RELOAD_TYPE_ALL;
    loadFontSystem(&fontLoad);

    waitForAllResourceLoads();

    return true;
}

void exitGraphics()
{
    waitQueueIdle(pGraphicsQueue);

    unloadFontSystem(RELOAD_TYPE_ALL);
    exitFontSystem();

    removeSwapChain(pRenderer, pSwapChain);
    exitGpuCmdRing(pRenderer, &gGraphicsCmdRing);
    exitSemaphore(pRenderer, pImageAcquiredSemaphore);

    exitRootSignature(pRenderer);
    exitResourceLoaderInterface(pRenderer);

    exitQueue(pRenderer, pGraphicsQueue);
    exitRenderer(pRenderer);
    exitGPUConfiguration();
    pRenderer = NULL;
}

// Consome no maximo um evento da fila por quadro (mesmo ritmo das cenas) e
// atualiza o estado da demo.
void consumeInput()
{
    const KeyEvent event = forgeui::readKey();
    switch (event.key)
    {
    case Key::None:
        return;
    case Key::Up:    gLastKey = "UP";    ++gArrowCount; break;
    case Key::Down:  gLastKey = "DOWN";  ++gArrowCount; break;
    case Key::Left:  gLastKey = "LEFT";  ++gArrowCount; break;
    case Key::Right: gLastKey = "RIGHT"; ++gArrowCount; break;
    case Key::Enter:
        gLastKey = "ENTER";
        gTyped.clear();
        break;
    case Key::Backspace:
        gLastKey = "BACKSPACE";
        if (!gTyped.empty())
        {
            gTyped.pop_back();
        }
        break;
    case Key::Escape:
        gLastKey = "ESC";
        break;
    case Key::Char:
        gLastKey = std::string(1, event.character);
        if (gTyped.size() < 40)
        {
            gTyped.push_back(event.character);
        }
        break;
    default:
        gLastKey = "?";
        break;
    }
}

void drawFrame()
{
    uint32_t swapchainImageIndex;
    acquireNextImage(pRenderer, pSwapChain, pImageAcquiredSemaphore, NULL, &swapchainImageIndex);

    RenderTarget*     pRenderTarget = pSwapChain->ppRenderTargets[swapchainImageIndex];
    GpuCmdRingElement elem = getNextGpuCmdRingElement(&gGraphicsCmdRing, true, 1);

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

    // texto via forgeui — a mesma API de desenho das cenas do 8PuzzleForge
    forgeui::beginDraw(cmd, (float)pRenderTarget->mWidth, (float)pRenderTarget->mHeight, gFontID);

    const float h = forgeui::screenHeight();
    forgeui::drawTextCentered("FORGE LIB SPIKE", h * 0.16f, 56.0f, forgeui::color::kTitle);
    forgeui::drawTextCentered("fontes + input proprios, sem IApp/WindowsMain — degrau 1", h * 0.16f + 76.0f, 22.0f,
                              forgeui::color::kDim);

    forgeui::drawTextCentered("digite algo: [ " + gTyped + "_ ]", h * 0.45f, 26.0f, forgeui::color::kAccent);
    forgeui::drawTextCentered("ultima tecla: " + gLastKey + "   |   setas pressionadas: " + std::to_string(gArrowCount),
                              h * 0.45f + 48.0f, 22.0f, forgeui::color::kValue);

    forgeui::drawHints("texto digita   BACKSPACE apaga   ENTER limpa   SETAS contam   ESC sai");

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
}

} // namespace

int main()
{
    // Mesma ordem de subsistemas do WindowsMain (WindowsBase.cpp:351).
    if (!initMemAlloc(kAppName))
        return EXIT_FAILURE;

    FileSystemInitDesc fsDesc = {};
    fsDesc.pAppName = kAppName;
    if (!initFileSystem(&fsDesc))
        return EXIT_FAILURE;

    initLog(kAppName, DEFAULT_LOG_LEVEL);

    const HWND hwnd = createSpikeWindow();
    if (!hwnd)
    {
        LOGF(eERROR, "[spike] falha ao criar a janela Win32");
        return EXIT_FAILURE;
    }

    // Cola do fontstash: monitores enumerados + gWindow apontando para o
    // nosso HWND ANTES do platformInitFontSystem (que le o DPI dali).
    if (!initWindowSystem())
    {
        LOGF(eERROR, "[spike] initWindowSystem falhou");
        return EXIT_FAILURE;
    }
    gWindowDesc.handle.type = WINDOW_HANDLE_TYPE_WIN32;
    gWindowDesc.handle.window = hwnd;
    gWindow = &gWindowDesc;

    if (!platformInitFontSystem())
    {
        LOGF(eERROR, "[spike] platformInitFontSystem falhou");
        return EXIT_FAILURE;
    }

    if (!initGraphics(hwnd))
    {
        return EXIT_FAILURE;
    }
    LOGF(eINFO, "[spike] renderer + fontes + input proprios, sem IApp/WindowsMain");

    ShowWindow(hwnd, SW_SHOW);

    while (!gQuit)
    {
        pumpMessages();
        if (gQuit)
        {
            break;
        }
        consumeInput();
        drawFrame();
    }

    exitGraphics();
    platformExitFontSystem();
    exitWindowSystem();

    UnregisterClassW(L"ForgeLibSpikeClass", GetModuleHandleW(NULL));

    exitLog();
    exitFileSystem();
    exitMemAlloc();
    return 0;
}
