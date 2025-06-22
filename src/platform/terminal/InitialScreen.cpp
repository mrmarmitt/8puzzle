#include "InitialScreen.h"

#include <iostream>

void InitialScreen::draw(ApplicationGame& applicationGame) {
    std::cout << "InitialScreen: draw" << std::endl;
    applicationGame.introduction();
}

void InitialScreen::input(ApplicationGame& applicationGame) {

}
