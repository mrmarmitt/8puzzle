#include "GameScreen.h"

#include <conio.h>
#include <iostream>

void GameScreen::draw(ApplicationGame& applicationGame) {

    std::cout << printBoard() << std::endl;

    if (m_board.isSolved())
        applicationGame.menu();
}

void GameScreen::input(ApplicationGame& applicationGame) {
    if (_kbhit()) {
        char key = _getch();

        if (key >= '0' && key <= '8') {
            const int numberPressed = key - '0';
            m_board.moveTile(numberPressed);
        }
    }
}

std::string GameScreen::printBoard() const {
    std::string result;

    result += "   0   1   2\n";
    result += " +---+---+---+\n";


    for (int line = 0; line < 3; ++line) {
        result += std::to_string(line) + "|";

        for (int column = 0; column < 3; ++column) {
            const int index = line * 3 + column;
            const Tile& tile = m_board.getBoard()[index];
            std::string value = tile.isEmpty() ? " " : tile.toString();

            result += " " + value + " |";
        }

        result += "\n +---+---+---+\n";
    }

    return result;
}