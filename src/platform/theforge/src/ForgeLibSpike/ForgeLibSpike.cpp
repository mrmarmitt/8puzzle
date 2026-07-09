// ForgeLibSpike — degrau 0 da fase 2 (.ai/task/02-theforge-fase2-library-mode.md).
//
// Prova a tese central do modo biblioteca: usar o The-Forge SEM IApp, SEM
// DEFINE_APPLICATION_MAIN e SEM WindowsMain. main() proprio cria uma janela
// Win32 crua, inicializa os subsistemas de base na ordem que o WindowsMain
// usa (memAlloc -> fileSystem -> log), sobe o renderer (initRenderer +
// addSwapChain(HWND)) e gira um loop minimo que limpa a tela de azul-noite.
//
// Deliberadamente FORA deste spike: resize (janela de tamanho fixo), fontes,
// input alem de ESC/fechar, cengine. Cada um entra num degrau posterior.

#include "Common_3/Graphics/Interfaces/IGraphics.h"
#include "Common_3/OS/Interfaces/IOperatingSystem.h"
#include "Common_3/Utilities/Interfaces/IFileSystem.h"
#include "Common_3/Utilities/Interfaces/ILog.h"

#include "Common_3/Utilities/RingBuffer.h"

#include "Common_3/Utilities/Interfaces/IMemory.h" // deve ser o ultimo include

// O DEFINE_APPLICATION_MAIN exporta estes simbolos para ativar o Agility SDK
// do D3D12 (D3D12Core.dll copiada para a pasta do exe). Sem IApp, exportamos
// por conta propria — D3D12_AGILITY_SDK_VERSION vem do TF_Shared.props.
extern "C"
{
    __declspec(dllexport) extern const UINT D3D12SDKVersion = D3D12_AGILITY_SDK_VERSION;
    __declspec(dllexport) extern const char* D3D12SDKPath = "";
}

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

bool gQuit = false;

// ---------------------------------------------------------------- janela

LRESULT CALLBACK spikeWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
        {
            PostMessageW(hwnd, WM_CLOSE, 0, 0);
            return 0;
        }
        break;
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

    return CreateWindowExW(0, wc.lpszClassName, L"Forge Lib Spike — degrau 0 (ESC sai)", style,
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

    return pSwapChain != NULL;
}

void exitGraphics()
{
    waitQueueIdle(pGraphicsQueue);

    removeSwapChain(pRenderer, pSwapChain);
    exitGpuCmdRing(pRenderer, &gGraphicsCmdRing);
    exitSemaphore(pRenderer, pImageAcquiredSemaphore);
    exitQueue(pRenderer, pGraphicsQueue);
    exitRenderer(pRenderer);
    exitGPUConfiguration();
    pRenderer = NULL;
}

// Um quadro: acquire -> clear (via load action) -> submit -> present.
// Sem resource loader (nada e carregado), o submit espera apenas o acquire.
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
    cmdBindRenderTargets(cmd, NULL);

    barriers[0] = { pRenderTarget, RESOURCE_STATE_RENDER_TARGET, RESOURCE_STATE_PRESENT };
    cmdResourceBarrier(cmd, 0, NULL, 0, NULL, 1, barriers);

    endCmd(cmd);

    QueueSubmitDesc submitDesc = {};
    submitDesc.mCmdCount = 1;
    submitDesc.mSignalSemaphoreCount = 1;
    submitDesc.mWaitSemaphoreCount = 1;
    submitDesc.ppCmds = &cmd;
    submitDesc.ppSignalSemaphores = &elem.pSemaphore;
    submitDesc.ppWaitSemaphores = &pImageAcquiredSemaphore;
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

    if (!initGraphics(hwnd))
    {
        return EXIT_FAILURE;
    }
    LOGF(eINFO, "[spike] renderer + swapchain de janela propria, sem IApp/WindowsMain");

    ShowWindow(hwnd, SW_SHOW);

    while (!gQuit)
    {
        pumpMessages();
        if (gQuit)
        {
            break;
        }
        drawFrame();
    }

    exitGraphics();

    UnregisterClassW(L"ForgeLibSpikeClass", GetModuleHandleW(NULL));

    exitLog();
    exitFileSystem();
    exitMemAlloc();
    return 0;
}
