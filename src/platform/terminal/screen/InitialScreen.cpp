#include "InitialScreen.h"

#include <iostream>

#include "8puzzle/game/GameRouter.h"

void InitialScreen::draw() {
    std::cout << "InitialScreen: draw" << std::endl;
    getGameRouter().introduction();
}
