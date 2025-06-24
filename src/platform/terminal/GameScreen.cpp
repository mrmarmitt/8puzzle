#include "GameScreen.h"

#include <conio.h>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

GameScreen::GameScreen(RecordService& service): m_recordService(&service){}

void GameScreen::draw(ApplicationGame& applicationGame) {

    if (!m_started) {
        m_startTime = std::chrono::steady_clock::now();
        m_started = true;
    }

    std::cout << printBoard() << std::endl;

    if (m_board.isSolved()) {
        auto record = Record(m_moves, m_time);

        if (m_recordService->isNewRecord(record, 10)) {
            record.assignRecord("name", "2025-06-22 18:10");
            m_recordService->addRecord(record);
        }
        applicationGame.menu();
    }
}

void GameScreen::input(ApplicationGame& applicationGame) {
    if (_kbhit()) {
        char key = _getch();

        if (key >= '0' && key <= '8') {
            const int numberPressed = key - '0';

            if (m_board.moveTile(numberPressed)) {
                m_moves = m_moves + 1;
            }
        }
    }
}

std::string GameScreen::printBoard() {
    std::ostringstream result;

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