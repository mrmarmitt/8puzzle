#include "ExitScene.h"

#include <iostream>

#include "8puzzle/engine/ServiceContainer.h"

ExitScene::ExitScene() :
    m_gameRouter(ServiceContainer::get().getRouter()){}

void ExitScene::draw() {
    std::cout << "ExitScreen: draw" << std::endl;
}

void ExitScene::input() {

}