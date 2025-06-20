#include "MenuScreen.h"

#include <conio.h>
#include <iostream>

void MenuScreen::draw(ApplicationGame& applicationGame) {
    std::cout << "MenuScreen: draw" << std::endl;
}

void MenuScreen::input(ApplicationGame& applicationGame) {
    if (_kbhit()) {
        int ch = _getch();
        if (ch == '\r') {
            applicationGame.exit();
        }
    }
}