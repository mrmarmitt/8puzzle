#include "GameScreen.h"

#include <conio.h>

#include <iostream>

void GameScreen::draw(ApplicationGame& applicationGame) {
    std::cout << m_board.toString() << std::endl;
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