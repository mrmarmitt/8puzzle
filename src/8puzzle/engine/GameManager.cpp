#include "GameManager.h"

#include <iostream>

#include "IScene.h"
#include "ServiceContainer.h"

GameManager::GameManager() : m_gameRouter(ServiceContainer::get().getRouter()){}
GameManager::~GameManager() = default;

void GameManager::onEnter() const {
    IScene& screen = m_gameRouter->getCurrentCachedScreen();

    if (!screen.isOnEnterExecuted()) {
        screen.onEnter();
        screen.onEnterExecuted();
    }
}

void GameManager::render() const {
    IScene& screen = m_gameRouter->getCurrentCachedScreen();

    screen.draw();
}

void GameManager::input() const {
    IScene& screen = m_gameRouter->getCurrentCachedScreen();

    screen.input();
}

void GameManager::onExit() const {
    if (m_gameRouter->hasNextScreen()) {
        IScene& screen = m_gameRouter->getCurrentCachedScreen();
        screen.onExit();
        m_gameRouter->goToNextScreen();
    }
}

bool GameManager::shouldExist() const {
    return m_gameRouter->getCurrentStateGameCode() == "exit";
}

void GameManager::cleanup() {
    std::cout << "TerminalGameManager: cleanup" << std::endl;
}


