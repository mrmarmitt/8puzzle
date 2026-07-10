// 8PuzzleForgeLib — degrau 3 da fase 2 (.ai/task/02-theforge-fase2-library-mode.md):
// o 8Puzzle completo com a CENGINE dona do loop e o The-Forge como BIBLIOTECA.
//
// E o mesmo jogo do 8PuzzleForge (fase 1, modo hospedado) com o casco
// trocado: sai o IApp/DEFINE_APPLICATION_MAIN, entra main() + EngineManager +
// TheForgeWindowManager (janela/GPU no par update()/present() da cengine
// 0.5.0). As cenas de 8PuzzleForge/scene/ sao reaproveitadas SEM MUDANCA —
// elas so falam com o forgeui, e a fila de teclado agora e alimentada pelo
// WndProc proprio (em vez do input system do framework).
//
// Fiacao identica ao main_ftxui.cpp: dominio, servicos, GameRouter e maquina
// de estados intocados; recordes no mesmo records.tsv.

// cengine + jogo — C++ puro, ANTES dos headers do The-Forge (IMemory.h por ultimo).
#include <cengine/core/EngineManager.hpp>
#include <cengine/routing/GameManager.hpp>
#include <cengine/routing/RouterInMemory.hpp>
#include <cengine/routing/SceneRepository.hpp>

#include "8puzzle/game/GameRouter.h"
#include "8puzzle/game/service/GamePlayService.h"
#include "8puzzle/game/service/RecordService.h"
#include "8puzzle/game/service/repository/FileRecordRepository.h"
#include "8puzzle/game/service/repository/GamePlayRepository.h"
#include "8puzzle/game/state/StateGame.h"

#include "8PuzzleForge/ForgeSceneFactory.h"
#include "ForgeLibSpike/TheForgeWindowManager.h"

#include <memory>

#include "Common_3/OS/Interfaces/IOperatingSystem.h" // UINT dos exports do Agility
#include "Common_3/Utilities/Interfaces/IFileSystem.h"
#include "Common_3/Utilities/Interfaces/ILog.h"

#include "Common_3/Utilities/Interfaces/IMemory.h" // deve ser o ultimo include

// O DEFINE_APPLICATION_MAIN exporta estes simbolos para ativar o Agility SDK
// do D3D12 (D3D12Core.dll copiada para a pasta do exe). Sem IApp, exportamos
// por conta propria — D3D12_AGILITY_SDK_VERSION vem do TF_Shared.props.
extern "C"
{
    __declspec(dllexport) extern const UINT D3D12SDKVersion = D3D12_AGILITY_SDK_VERSION;
    __declspec(dllexport) extern const char* D3D12SDKPath = "";
}

int main()
{
    constexpr const char* kAppName = "8PuzzleForgeLib";

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
        // fiacao do jogo (identica ao main_ftxui.cpp e ao Init() da fase 1)
        auto sceneRepository = std::make_unique<cengine::routing::SceneRepository>();
        cengine::routing::ISceneRepository& sceneRepositoryRef = *sceneRepository;

        const auto router = std::make_shared<cengine::routing::RouterInMemory>(
            std::move(sceneRepository), std::make_unique<InitialSG>());

        const auto gameRouter = std::make_shared<GameRouter>(router);

        const auto gamePlayRepository = std::make_shared<GamePlayRepository>();
        const auto gamePlayService = std::make_shared<GamePlayService>(gamePlayRepository);
        // Recordes persistidos no mesmo arquivo das outras plataformas
        // (relativo ao diretorio de trabalho — o OutDir do exe).
        const auto recordRepository = std::make_shared<FileRecordRepository>("records.tsv");
        const auto recordService = std::make_shared<RecordService>(recordRepository);

        ForgeSceneFactory::populateForgeScenes(sceneRepositoryRef, gameRouter, gamePlayService, recordService);

        // Modo PROPRIO (diferenca para a fase 1): a cengine dirige o loop e o
        // The-Forge entra como biblioteca atras do IWindowManager.
        cengine::core::EngineManager engine{
            std::make_unique<TheForgeWindowManager>(kAppName, 1280, 720),
            std::make_unique<cengine::routing::GameManager>(router),
        };

        engine.start(); // bloqueia ate o jogo rotear para "exit"; o
                        // cleanup() (jogo + janela/GPU) roda no fim do start()
    }

    LOGF(eINFO, "[8puzzle] loop da cengine encerrado");

    exitLog();
    exitFileSystem();
    exitMemAlloc();
    return 0;
}
