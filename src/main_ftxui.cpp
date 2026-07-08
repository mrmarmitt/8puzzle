#include <iostream>
#include <memory>

#include <cengine/core/EngineManager.hpp>
#include <cengine/routing/SceneRepository.hpp>
#include <cengine/routing/RouterInMemory.hpp>
#include <cengine/routing/GameManager.hpp>

#include "8puzzle/game/GameRouter.h"
#include "8puzzle/game/service/GamePlayService.h"
#include "8puzzle/game/service/RecordService.h"
#include "8puzzle/game/service/repository/GamePlayRepository.h"
#include "8puzzle/game/state/StateGame.h"

#include "platform/ftxui/FtxuiSceneFactory.h"
#include "platform/ftxui/FtxuiWindowManager.h"

// PoC da interface FTXUI: mesma fiação do main.cpp (cengine 0.3.0), trocando
// apenas a PLATAFORMA — window manager e cenas. Domínio e estados intocados.
int main()
{
    auto sceneRepository = std::make_unique<cengine::routing::SceneRepository>();
    cengine::routing::ISceneRepository& sceneRepositoryRef = *sceneRepository;

    const auto router = std::make_shared<cengine::routing::RouterInMemory>(
        std::move(sceneRepository), std::make_unique<InitialSG>());

    const auto gameRouter = std::make_shared<GameRouter>(router);

    const auto gamePlayRepository = std::make_shared<GamePlayRepository>();
    const auto gamePlayService = std::make_shared<GamePlayService>(gamePlayRepository);
    const auto recordService = std::make_shared<RecordService>();

    FtxuiSceneFactory::populateFtxuiScenes(sceneRepositoryRef, gameRouter, gamePlayService, recordService);

    cengine::core::EngineManager engineManager(
        std::make_unique<FtxuiWindowManager>(),
        std::make_unique<cengine::routing::GameManager>(router)
    );

    engineManager.start();

    std::cout << "Jogo encerrado." << std::endl;
    return 0;
}
