#include "InitialScene.h"

#include <iostream>

#include "8puzzle/game/GameRouter.h"

void InitialScene::draw() {
    std::cout << "InitialScreen: draw" << std::endl;
    getGameRouter().introduction();
}
