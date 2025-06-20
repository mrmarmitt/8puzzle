#include "IntroductionScreen.h"

#include <conio.h>
#include <iostream>

void IntroductionScreen::draw(ApplicationGame& applicationGame) {
    std::cout << "IntroductionScreen: draw" << std::endl;
}

void IntroductionScreen::input(ApplicationGame& applicationGame) {
    if (_kbhit()) {
        int ch = _getch();
        if (ch == '\r') {
            applicationGame.menu();
        }
    }
}