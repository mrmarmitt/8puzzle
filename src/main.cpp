#include <iostream>

#include "8puzzle/core/Board.h"
#include "platform/EngineManager.h"
#include "platform/FPSManager.h"

#include "8puzzle/domain/ApplicationGame.h"
#include "platform/terminal/TerminalGameManager.h"
#include "platform/terminal/TerminalWindowManager.h"

using namespace std;


int main()
{
    std::cout << "Iniciando o jogo..." << std::endl;
    auto windowManager = std::make_unique<TerminalWindowManager>();
    auto fpsManager = std::make_unique<FPSManager>(20);
    auto gameManager = std::make_unique<TerminalGameManager>();

    const auto applicationGame = std::make_unique<ApplicationGame>();

    EngineManager engineManager(
        std::move(windowManager),
        std::move(gameManager),
        std::move(fpsManager)
    );

    engineManager.start();

    std::cout << "Jogo encerrado." << std::endl;
	return 0;
}