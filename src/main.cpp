#include <iostream>

#include "8puzzle/core/Board.h"
#include "platform/EngineManager.h"
#include "platform/FPSManager.h"

#include <platform/IWindowManager.h>

#include "platform/domain/ApplicationGame.h"
#include "platform/terminal/TerminalGameManager.h"

using namespace std;

class DummyWindowManager final : public IWindowManager {
public:
    void init() override { std::cout << "DummyWindowManager: Init" << std::endl; }
    void update() override { /* std::cout << "DummyWindowManager: Update" << std::endl; */ }
    void cleanup() override { std::cout << "DummyWindowManager: cleanup" << std::endl; }
    ~DummyWindowManager() override = default;
};


int main()
{
    std::cout << "Iniciando o jogo..." << std::endl;
    auto windowManager = std::make_unique<DummyWindowManager>();
    auto fpsManager = std::make_unique<FPSManager>(2);
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