#include "TerminalGameManager.h"

#include <iostream>
#include <string>

#include "ExitScreen.h"
#include "InitialScreen.h"
#include "IntroductionScreen.h"
#include "MenuScreen.h"
#include "platform/domain/ApplicationGame.h"


TerminalGameManager::TerminalGameManager() {
    m_applicationGame = std::make_unique<ApplicationGame>();
    m_screenCache = std::make_unique<ScreenCache>();

    m_screenCache->registerScreen(Initial, []() {
        return std::make_unique<InitialScreen>();
    });

    m_screenCache->registerScreen(Introduction, []() {
        return std::make_unique<IntroductionScreen>();
    });

    m_screenCache->registerScreen(Menu, []() {
        return std::make_unique<MenuScreen>();
    });

    m_screenCache->registerScreen(Exit, []() {
        return std::make_unique<ExitScreen>();
    });

}

void TerminalGameManager::render() {
    const GameState gameState = m_applicationGame->getGameState();
    IScreen& screen = m_screenCache->getScreen(gameState);

    screen.draw(*m_applicationGame);
}

bool TerminalGameManager::shouldExist() {
    return m_applicationGame->getGameState() == Exit;
}

void TerminalGameManager::input() {
    const GameState gameState = m_applicationGame->getGameState();
    IScreen& screen = m_screenCache->getScreen(gameState);

    screen.input(*m_applicationGame);
}

void TerminalGameManager::cleanup() {
    std::cout << "TerminalGameManager: cleanup" << std::endl;
}


