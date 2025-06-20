#include "InitialScreen.h"

#include <conio.h>
#include <iostream>

void InitialScreen::draw(ApplicationGame& applicationGame) {
    std::cout << "InitialScreen: draw" << std::endl;
}

void InitialScreen::input(ApplicationGame& applicationGame) {
    if (_kbhit()) {
        int ch = _getch();
        if (ch == '\r') {
            applicationGame.introduction();
        }
    }
}
