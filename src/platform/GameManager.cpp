#include "GameManager.h"

#include <iostream>

#include "8puzzle/domain/GameRouter.h"
#include "platform/IScreen.h"

GameManager::~GameManager() = default;

void GameManager::onEnter() const {
    IScreen& screen = m_gameRouter->getCurrentCachedScreen();

    screen.onEnter();
    screen.onEnterExecuted();
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
    IScreen& screen = m_gameRouter->getCurrentCachedScreen();

    screen.onEnter();
    screen.onExitExecuted();
}

bool GameManager::shouldExist() const {
    return m_gameRouter->getCurrentStateGameEnum() == StateEnum::Exit;
}

void GameManager::cleanup() {
    std::cout << "TerminalGameManager: cleanup" << std::endl;
}


