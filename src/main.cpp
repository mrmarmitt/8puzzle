#include <iostream>
#include <memory>

#include <cengine/core/EngineManager.hpp>
#include <cengine/routing/SceneRepository.hpp>
#include <cengine/routing/RouterInMemory.hpp>
#include <cengine/routing/GameManager.hpp>

#include "8puzzle/game/GameRouter.h"
#include "8puzzle/game/service/ConfigurationService.h"
#include "8puzzle/game/service/GamePlayService.h"
#include "8puzzle/game/service/RecordService.h"
#include "8puzzle/game/service/repository/GamePlayRepository.h"
#include "8puzzle/game/state/StateGame.h"

#include "platform/terminal/TerminalSceneFactory.h"
#include "platform/terminal/TerminalWindowManager.h"


int main()
{
    std::cout << "Iniciando o jogo..." << std::endl;

    // Repositório de cenas da cengine, semeado com o estado inicial do jogo.
    const auto sceneRepository = std::make_shared<cengine::routing::SceneRepository>(std::make_unique<InitialSG>());
    // Repositório da GamePlay
    const auto gamePlayRepository = std::make_shared<GamePlayRepository>();

    // Fachada de navegação de domínio usada pelas cenas, sobre o mesmo repositório.
    const auto gameRouter = std::make_shared<GameRouter>(sceneRepository);
    // Services do jogo
    const auto configurationService = std::make_shared<ConfigurationService>();
    const auto gamePlayService = std::make_shared<GamePlayService>(gamePlayRepository);
    const auto recordService = std::make_shared<RecordService>();

    // Popula o repositório com as factories das cenas do terminal.
    TerminalSceneFactory::populateTerminalScreens(sceneRepository, gameRouter, configurationService, gamePlayService, recordService);

    auto windowManager = std::make_unique<TerminalWindowManager>();

    // Roteador que o game loop dirige. Compartilha o MESMO sceneRepository do
    // gameRouter, então as navegações agendadas pelas cenas são vistas aqui.
    auto router = std::make_shared<cengine::routing::RouterInMemory>(sceneRepository);
    auto gameManager = std::make_unique<cengine::routing::GameManager>(router);

    cengine::core::EngineManager engineManager(
        std::move(windowManager),
        std::move(gameManager)
    );

    engineManager.start();

    std::cout << "Jogo encerrado." << std::endl;
    return 0;
}
