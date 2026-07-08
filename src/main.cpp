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

    // Provedor de cenas da cengine. A posse vai para o router logo abaixo
    // (posse exclusiva, cengine 0.2.0); a referência fica só para registrar as
    // factories durante a montagem — o endereço é estável após o move.
    auto sceneRepository = std::make_unique<cengine::routing::SceneRepository>();
    cengine::routing::ISceneRepository& sceneRepositoryRef = *sceneRepository;

    // Roteador que o game loop dirige: dono do repositório e da máquina de
    // estados, semeado com o estado inicial do jogo.
    const auto router = std::make_shared<cengine::routing::RouterInMemory>(
        std::move(sceneRepository), std::make_unique<InitialSG>());

    // Fachada de navegação de domínio usada pelas cenas, sobre o mesmo router.
    const auto gameRouter = std::make_shared<GameRouter>(router);

    // Repositório da GamePlay e services do jogo
    const auto gamePlayRepository = std::make_shared<GamePlayRepository>();
    const auto configurationService = std::make_shared<ConfigurationService>();
    const auto gamePlayService = std::make_shared<GamePlayService>(gamePlayRepository);
    const auto recordService = std::make_shared<RecordService>();

    // Popula o repositório com as factories das cenas do terminal (roda antes
    // do primeiro frame; as factories capturam suas dependências por valor).
    TerminalSceneFactory::populateTerminalScreens(sceneRepositoryRef, gameRouter, configurationService, gamePlayService, recordService);

    auto windowManager = std::make_unique<TerminalWindowManager>();
    auto gameManager = std::make_unique<cengine::routing::GameManager>(router);

    cengine::core::EngineManager engineManager(
        std::move(windowManager),
        std::move(gameManager)
    );

    engineManager.start();

    std::cout << "Jogo encerrado." << std::endl;
    return 0;
}
