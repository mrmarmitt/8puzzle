#pragma once

// TheForgeWindowManager — degrau 2 da fase 2 (.ai/task/02-theforge-fase2-
// library-mode.md): o The-Forge como BIBLIOTECA atras do port IWindowManager
// da cengine. A cengine e dona do loop (EngineManager::start()); este window
// manager encapsula TUDO de janela/GPU no par update()/present() da 0.5.0:
//
//   init()     janela Win32 propria + cola do fontstash + renderer + fontes
//              + swapchain (a receita validada nos degraus 0 e 1)
//   update()   pump de mensagens (WndProc alimenta a fila do forgeui),
//              resize pendente (recria swapchain), acquire da imagem,
//              beginCmd + barriers + forgeui::beginDraw
//   present()  fecha o command buffer, submit e queuePresent
//   cleanup()  teardown na ordem inversa
//
// As cenas nao conhecem esta classe: desenham/leem teclado via forgeui, o
// mesmo contrato do modo hospedado (8PuzzleForge).

#include <cstdint>

#include <cengine/core/IWindowManager.hpp>

class TheForgeWindowManager final : public cengine::core::IWindowManager {
public:
    /// @param appName nome do app (initRenderer, titulo da janela).
    /// @param width/height tamanho INICIAL do client area; a janela e
    ///        redimensionavel (WM_SIZE recria o swapchain no update()).
    TheForgeWindowManager(const char* appName, int32_t width, int32_t height);

    void init() override;
    void update() override;
    void present() override;
    void cleanup() override;

private:
    bool initGraphics();
    void applyPendingResize();

    const char* m_appName;
    int32_t     m_width;
    int32_t     m_height;
};
