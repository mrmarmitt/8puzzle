#include "InitialScene.h"

#include <iostream>

#include "8puzzle/game/GameRouter.h"

InitialScene::InitialScene(std::shared_ptr<GameRouter> gameRouter) : m_gameRouter(std::move(gameRouter)) {}

void InitialScene::draw() {
    std::cout << "InitialScreen: draw" << std::endl;
    m_gameRouter->introduction();
}
