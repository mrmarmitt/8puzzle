#include "GameScene.h"

#include <conio.h>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "8puzzle/game/GameRouter.h"
#include "8puzzle/game/service/GamePlayService.h"

GameScene::GameScene(std::shared_ptr<GameRouter> gameRouter, std::shared_ptr<GamePlayService> gamePlayService)
    : m_gameRouter(std::move(gameRouter)), m_gamePlayService(std::move(gamePlayService)) {}


void GameScene::draw() {
    auto& gamePlay = m_gamePlayService->getGamePlay();
    std::cout << printBoard() << std::endl;

    if (m_board.isSolved()) {
        gamePlay.completeGame();
        m_gameRouter->gameOver();
    }
}

void GameScene::input() {
    auto& gamePlay = m_gamePlayService->getGamePlay();
    if (_kbhit()) {
        const int key = _getch();

        if (key >= '0' && key <= '8') {
            const int numberPressed = key - '0';

            if (m_board.moveTile(numberPressed)) {
                gamePlay.incrementMove();
            }
        }

        if (key == 27) {
            m_gameRouter->menu();
        }
    }
}

void GameScene::onExit() {
    m_board.createRandomGame();
}

std::string GameScene::printBoard() const {
    const auto& gamePlay = m_gamePlayService->getGamePlay();
    std::ostringstream result;

    result << "=== GAME ===\n";
    result << "(1-8) Movimentos | (ESC) Voltar\n\n";

    result << "   0   1   2\n";
    result << " +---+---+---+\n";

    for (int line = 0; line < 3; ++line) {
        result << line << "|";

        for (int column = 0; column < 3; ++column) {
            const int index = line * 3 + column;
            const Tile &tile = m_board.getBoard()[index];
            std::string value = tile.isEmpty() ? " " : tile.toString();

            result << " " << value << " |";
        }

        result << "\n +---+---+---+\n";
    }

    const auto now = std::chrono::system_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - gamePlay.getStartedAt()
    ).count();

    const int hours = static_cast<int>(elapsed / (1000 * 60 * 60));
    const int minutes = static_cast<int>((elapsed / (1000 * 60)) % 60);
    const int seconds = static_cast<int>((elapsed / 1000) % 60);
    const int milliseconds = static_cast<int>(elapsed % 1000);

    result << "\nMovimentos: " << gamePlay.getNumberOfMoves() << "\n";
    result << "Tempo: " << std::setfill('0') << std::setw(2) << hours << ":" << std::setw(2)
            << minutes << ":" << std::setw(2) << seconds << "." << std::setw(3) << milliseconds
            << "\n";

    return result.str();
}
