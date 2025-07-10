#include "InitialScene.h"

#include <iostream>

#include "8puzzle/engine/ServiceContainer.h"

InitialScene::InitialScene() :
    m_gameRouter(ServiceContainer::get().getRouter()) {}

void InitialScene::draw() {
    std::cout << "InitialScreen: draw" << std::endl;
    m_gameRouter->introduction();
}
