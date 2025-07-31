#include <iostream>

#include "8puzzle/engine/EngineManager.h"

#include "8puzzle/engine/service/repository/SceneRepository.h"
#include "8puzzle/game/GameRouter.h"
#include "8puzzle/game/service/ConfigurationService.h"
#include "8puzzle/game/service/GamePlayService.h"
#include "8puzzle/game/service/RecordService.h"
#include "8puzzle/game/service/repository/GamePlayRepository.h"
#include "8puzzle/game/state/StateGame.h"
#include "platform/terminal/TerminalSceneFactory.h"
#include "platform/terminal/TerminalWindowManager.h"

using namespace std;


int main()
{
    std::cout << "Iniciando o jogo..." << std::endl;

    //Iniciando o repository de Scenas
    const auto sceneRepository = std::make_shared<SceneRepository>(std::make_unique<InitialSG>());
    //Iniciando o repository da GamePlay
    const auto gamePlayRepository = std::make_shared<GamePlayRepository>();

    //Iniciando a service de gerenciamento de scenas.
    const auto gameRouter = std::make_shared<GameRouter>(sceneRepository);
    //Iniciando a service de gerenciamento da configuração
    const auto configurationService = std::make_shared<ConfigurationService>();
    //Iniciando a service de gerenciamento da gamePlay
    const auto gamePlayService = std::make_shared<GamePlayService>(gamePlayRepository);
    //Iniciando a service de gerenciamento do recorde
    const auto recordService = std::make_shared<RecordService>();

    //Populando o repository com os metodos construtores das scenas
    TerminalSceneFactory::populateTerminalScreens(sceneRepository, gameRouter, configurationService, gamePlayService, recordService);

    //Iniciando o terminal
    auto windowManager = std::make_unique<TerminalWindowManager>();
    //Iniciando o GameManager
    auto gameManager = std::make_unique<GameManager>(gameRouter);

    EngineManager engineManager(
        std::move(windowManager),
        std::move(gameManager)
    );

    engineManager.start();

    std::cout << "Jogo encerrado." << std::endl;
	return 0;
}