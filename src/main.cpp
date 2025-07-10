#include <iostream>

#include "8puzzle/engine/EngineManager.h"
#include "8puzzle/engine/FPSManager.h"

#include "8puzzle/engine/SceneContainer.h"
#include "8puzzle/game/GameRouter.h"
#include "platform/terminal/TerminalSceneFactory.h"
#include "platform/terminal/TerminalWindowManager.h"

using namespace std;


int main()
{
    std::cout << "Iniciando o jogo..." << std::endl;

    SceneContainer& container = SceneContainer::get();

    TerminalSceneFactory::populateTerminalScreens(container);


    auto windowManager = std::make_unique<TerminalWindowManager>();
    auto fpsManager = std::make_unique<FPSManager>(30);
    auto gameManager = std::make_unique<GameManager>();

    EngineManager engineManager(
        std::move(windowManager),
        std::move(gameManager),
        std::move(fpsManager)
    );

    engineManager.start();

    std::cout << "Jogo encerrado." << std::endl;
	return 0;
}