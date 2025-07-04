#include "GameManager.h"

#include <iostream>

#include "8puzzle/game/GameRouter.h"
#include "IScreen.h"

GameManager::~GameManager() = default;

void GameManager::onEnter() const {
    IScreen& screen = m_gameRouter->getCurrentCachedScreen();

    if (!screen.isOnEnterExecuted()) {
        screen.onEnter();
        screen.onEnterExecuted();
    }
}

void GameManager::render() const {
    IScreen& screen = m_gameRouter->getCurrentCachedScreen();

    screen.draw();
}

void GameManager::input() const {
    IScreen& screen = m_gameRouter->getCurrentCachedScreen();

    screen.input();
}

void GameManager::onExit() const {
    if (m_gameRouter->hasNextScreen()) {
        IScreen& screen = m_gameRouter->getCurrentCachedScreen();
        screen.onExit();
        screen.resetConst();
        m_gameRouter->goToNextScreen();
    }
}

bool GameManager::shouldExist() const {
    return m_gameRouter->getCurrentStateGameCode() == "exit";
}

void GameManager::cleanup() {
    std::cout << "TerminalGameManager: cleanup" << std::endl;
}


