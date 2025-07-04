#include <iostream>

#include "8puzzle/engine/EngineManager.h"
#include "8puzzle/engine/FPSManager.h"

#include "8puzzle/engine/ScreenContainer.h"
#include "8puzzle/game/GameRouter.h"
#include "platform/terminal/TerminalScreenFactory.h"
#include "platform/terminal/TerminalWindowManager.h"

using namespace std;


int main()
{
    std::cout << "Iniciando o jogo..." << std::endl;

    const auto gameRouter = std::make_unique<GameRouter>();
    ScreenContainer& container = ScreenContainer::get();

    TerminalScreenFactory::populateTerminalScreens(container, *gameRouter);


    auto windowManager = std::make_unique<TerminalWindowManager>();
    auto fpsManager = std::make_unique<FPSManager>(30);
    auto gameManager = std::make_unique<GameManager>(*gameRouter);

    EngineManager engineManager(
        std::move(windowManager),
        std::move(gameManager),
        std::move(fpsManager)
    );

    engineManager.start();

    std::cout << "Jogo encerrado." << std::endl;
	return 0;
}