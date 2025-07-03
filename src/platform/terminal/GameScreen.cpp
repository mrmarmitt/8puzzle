#include "GameScreen.h"

#include <conio.h>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "8puzzle/domain/GameRouter.h"
#include "platform/ServiceContainer.h"

GameScreen::GameScreen(GameRouter& gameRouter) :
    IScreen(gameRouter),
    m_gamePlayService(ServiceContainer::get().getGameService()){}


void GameScreen::draw() {
    const auto gamePlay = m_gamePlayService->getCurrentGamePlay();
    std::cout << printBoard() << std::endl;

    if (m_board.isSolved()) {
        gamePlay->completeGame();
        getGameRouter().gameOver();
    }
}

void GameScreen::input() {
    const auto gamePlay = m_gamePlayService->getCurrentGamePlay();
    if (_kbhit()) {
        const int key = _getch();

        if (key >= '0' && key <= '8') {
            const int numberPressed = key - '0';

            if (m_board.moveTile(numberPressed)) {
                gamePlay->incrementMove();
            }
        }

        if (key == 27) {
            m_gamePlayService->clear();
            getGameRouter().menu();
        }
    }
}

void GameScreen::onExit() {
    std::cout << "GAME_SCREEN PASSOU AQUI" << std::endl;
    m_board.createRandomGame();
}

std::string GameScreen::printBoard() const {
    const auto gamePlay = m_gamePlayService->getCurrentGamePlay();
    std::ostringstream result;

    result << "=== GAME ===\n";
    result << "(1-8) Movimentos | (ESC) Voltar\n\n";

    result << "   0   1   2\n";
    result << " +---+---+---+\n";

    for (int line = 0; line < 3; ++line) {
        result << line << "|";

        for (int column = 0; column < 3; ++column) {
            const int index = line * 3 + column;
            const Tile& tile = m_board.getBoard()[index];
            std::string value = tile.isEmpty() ? " " : tile.toString();

            result << " " << value << " |";
        }

        result << "\n +---+---+---+\n";
    }

    const auto now = std::chrono::system_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - gamePlay->getStartedAt()
    ).count();

    const int hours = static_cast<int>(elapsed / (1000 * 60 * 60));
    const int minutes = static_cast<int>((elapsed / (1000 * 60)) % 60);
    const int seconds = static_cast<int>((elapsed / 1000) % 60);
    const int milliseconds = static_cast<int>(elapsed % 1000);

    result << "\nMovimentos: " << gamePlay->getNumberOfMoves() << "\n";
    result << "Tempo: " << std::setfill('0') << std::setw(2) << hours << ":" << std::setw(2)
           << minutes << ":" << std::setw(2) << seconds << "." << std::setw(3) << milliseconds
           << "\n";

    return result.str();
}
