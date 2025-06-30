#include "GameScreen.h"

#include <conio.h>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "8puzzle/domain/GameRouter.h"
#include "8puzzle/domain/Record.h"

void GameScreen::draw() {

    if (!m_started) {
        m_startTime = std::chrono::steady_clock::now();
        m_started = true;
    }

    std::cout << printBoard() << std::endl;

    if (m_board.isSolved()) {
        const auto record = Record(m_moves, m_time);
        // gameRouter.setRecordFromEndGame(record);
        getGameRouter().gameOver();
    }
}

void GameScreen::input() {
    if (_kbhit()) {
        char key = _getch();

        if (key >= '0' && key <= '8') {
            const int numberPressed = key - '0';

            if (m_board.moveTile(numberPressed)) {
                m_moves = m_moves + 1;
            }
        }

        if (key == 27) {
            m_moves = 0;
            m_started = false;
            getGameRouter().menu();
        }
    }
}

std::string GameScreen::printBoard() {
    std::ostringstream result;

    result << "=== RECORDES ===\n";
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

    const auto now = std::chrono::steady_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_startTime).count();
    m_time = static_cast<int>(elapsed);

    const int hours = static_cast<int>(elapsed / (1000 * 60 * 60));
    const int minutes = static_cast<int>((elapsed / (1000 * 60)) % 60);
    const int seconds = static_cast<int>((elapsed / 1000) % 60);
    const int milliseconds = static_cast<int>(elapsed % 1000);

    result << "\nMovimentos: " << m_moves << "\n";
    result << "Tempo: "
           << std::setfill('0') << std::setw(2) << hours << ":"
           << std::setw(2) << minutes << ":"
           << std::setw(2) << seconds << "."
           << std::setw(3) << milliseconds << "\n";

    return result.str();
}