// ForgeLibSpike — degraus 0, 1 e 2 da fase 2 (.ai/task/02-theforge-fase2-library-mode.md).
//
// The-Forge SEM IApp, SEM DEFINE_APPLICATION_MAIN e SEM WindowsMain:
// - degrau 0: janela Win32 propria + initRenderer + addSwapChain(HWND);
// - degrau 1: fontes (fontstash) e input proprio na fila do forgeui;
// - degrau 2 (este arquivo): a CENGINE assume o loop — main() faz a mesma
//   fiacao do main_ftxui.cpp (repository -> router -> GameManager ->
//   EngineManager::start()) e todo o boilerplate de janela/GPU vive no
//   TheForgeWindowManager (update() abre o quadro, present() fecha — task 16
//   da cengine, 0.5.0). Duas cenas de teste navegam entre si e provam o
//   fixed timestep; a janela e redimensionavel (swapchain recriado).
//
// Ainda FORA do spike: o jogo completo (degrau 3).

#include "Common_3/OS/Interfaces/IOperatingSystem.h" // UINT dos exports do Agility
#include "Common_3/Utilities/Interfaces/IFileSystem.h"
#include "Common_3/Utilities/Interfaces/ILog.h"

#include <cengine/core/EngineManager.hpp>
#include <cengine/routing/GameManager.hpp>
#include <cengine/routing/RouterInMemory.hpp>
#include <cengine/routing/SceneRepository.hpp>
#include <cengine/routing/StateCodes.hpp>

#include "8PuzzleForge/ForgeUi.h"
#include "8PuzzleForge/TheForgeWindowManager.h"

#include <cstdio>
#include <memory>
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

namespace {

constexpr const char* kAppName = "ForgeLibSpike";

// ---------------------------------------------------------------- estados

// Estado generico do spike: so codigo + nome (o fluxo A <-> B nao precisa da
// state machine rica do jogo).
class SpikeState final : public cengine::routing::IState {
public:
    SpikeState(std::string code, std::string name) : m_code(std::move(code)), m_name(std::move(name)) {}

    [[nodiscard]] std::string getCode() const override { return m_code; }
    [[nodiscard]] std::string getName() const override { return m_name; }

private:
    std::string m_code;
    std::string m_name;
};

// ---------------------------------------------------------------- cenas

// Cena de teste: mostra o proprio nome, um relogio SIMULADO (avancado so em
// update(dt) — prova do fixed timestep: 60 passos/s independentemente do
// vsync) e o tamanho atual da janela (prova do resize). ENTER navega para a
// outra cena, ESC (ou o X da janela) roteia para o estado de saida.
class SpikeScene final : public cengine::core::IScene {
public:
    SpikeScene(std::shared_ptr<cengine::routing::IRouter> router, std::string title, std::string nextCode,
               std::string nextName, uint32_t titleColor) :
        m_router(std::move(router)), m_title(std::move(title)), m_nextCode(std::move(nextCode)),
        m_nextName(std::move(nextName)), m_titleColor(titleColor)
    {
    }

    void onEnter() override
    {
        m_simTime = 0.0;
        m_steps = 0;
    }

    void input() override
    {
        const KeyEvent event = forgeui::readKey();
        switch (event.key)
        {
        case Key::Enter:
            m_router->requestState(std::make_unique<SpikeState>(m_nextCode, m_nextName));
            break;
        case Key::Escape:
            m_router->requestState(
                std::make_unique<SpikeState>(std::string(cengine::routing::kExitStateCode), "Exit"));
            break;
        default:
            break;
        }
    }

    void update(const cengine::core::Seconds dt) override
    {
        m_simTime += dt.count();
        ++m_steps;
    }

    void draw() override
    {
        const float h = forgeui::screenHeight();

        forgeui::drawTextCentered(m_title, h * 0.16f, 56.0f, m_titleColor);
        forgeui::drawTextCentered("cengine dona do loop (start), The-Forge como biblioteca — degrau 2",
                                  h * 0.16f + 76.0f, 22.0f, forgeui::color::kDim);

        // relogio simulado: passos * dt fixo — anda a 60 passos/s mesmo se o
        // quadro real variar (acumulador do EngineManager)
        const char* spinner = "|/-\\";
        char        line[96];
        (void)snprintf(line, sizeof(line), "%c  tempo simulado: %.2f s  (%u passos de 1/60 s)",
                       spinner[(m_steps / 15) % 4], m_simTime, m_steps);
        forgeui::drawTextCentered(line, h * 0.45f, 26.0f, forgeui::color::kValue);

        (void)snprintf(line, sizeof(line), "janela: %.0f x %.0f  (arraste a borda: o swapchain e recriado)",
                       forgeui::screenWidth(), h);
        forgeui::drawTextCentered(line, h * 0.45f + 48.0f, 22.0f, forgeui::color::kText);

        forgeui::drawHints("ENTER troca de cena   ESC ou X sai");
    }

    void onExit() override {}

private:
    std::shared_ptr<cengine::routing::IRouter> m_router;
    std::string m_title;
    std::string m_nextCode;
    std::string m_nextName;
    uint32_t    m_titleColor;

    double   m_simTime = 0.0;
    uint32_t m_steps = 0;
};

// Cena do estado de saida: o commit para "exit" a instancia via factory, mas
// o loop termina no mesmo quadro (shouldExit) — nada a fazer.
class SpikeExitScene final : public cengine::core::IScene {
public:
    void onEnter() override {}
    void input() override {}
    void update(cengine::core::Seconds) override {}
    void draw() override {}
    void onExit() override {}
};

} // namespace

int main()
{
    // Subsistemas de processo na ordem do WindowsMain; o resto (janela, GPU,
    // fontes) e responsabilidade do TheForgeWindowManager, dentro da cengine.
    if (!initMemAlloc(kAppName))
        return EXIT_FAILURE;

    FileSystemInitDesc fsDesc = {};
    fsDesc.pAppName = kAppName;
    if (!initFileSystem(&fsDesc))
        return EXIT_FAILURE;

    initLog(kAppName, DEFAULT_LOG_LEVEL);

    {
        // Mesma fiacao do main_ftxui.cpp, trocando apenas a plataforma.
        auto sceneRepository = std::make_unique<cengine::routing::SceneRepository>();
        cengine::routing::ISceneRepository& repositoryRef = *sceneRepository;

        const auto router = std::make_shared<cengine::routing::RouterInMemory>(
            std::move(sceneRepository), std::make_unique<SpikeState>("scene_a", "Scene A"));

        repositoryRef.registerFactory("scene_a", [router] {
            return std::make_unique<SpikeScene>(router, "CENA A", "scene_b", "Scene B", forgeui::color::kTitle);
        });
        repositoryRef.registerFactory("scene_b", [router] {
            return std::make_unique<SpikeScene>(router, "CENA B", "scene_a", "Scene A", forgeui::color::kAccent);
        });
        repositoryRef.registerFactory(std::string(cengine::routing::kExitStateCode),
                                      [] { return std::make_unique<SpikeExitScene>(); });

        cengine::core::EngineManager engine{
            std::make_unique<TheForgeWindowManager>(kAppName, 1280, 720),
            std::make_unique<cengine::routing::GameManager>(router),
        };

        engine.start(); // bloqueia ate uma cena rotear para "exit"; o
                        // cleanup() (jogo + janela/GPU) roda no fim do start()
    }

    LOGF(eINFO, "[spike] loop da cengine encerrado");

    exitLog();
    exitFileSystem();
    exitMemAlloc();
    return 0;
}
