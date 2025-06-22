#include "TerminalGameManager.h"

#include <iostream>

#include "ExitScreen.h"
#include "GameScreen.h"
#include "InitialScreen.h"
#include "IntroductionScreen.h"
#include "MenuScreen.h"
#include "platform/domain/ApplicationGame.h"


TerminalGameManager::TerminalGameManager() {
    m_applicationGame = std::make_unique<ApplicationGame>();
    m_screenCache = std::make_unique<ScreenCache>();

    m_screenCache->registerScreen(StateEnum::Initial, []() {
        return std::make_unique<InitialScreen>();
    });

    m_screenCache->registerScreen(StateEnum::Introduction, []() {
        return std::make_unique<IntroductionScreen>();
    });

    m_screenCache->registerScreen(StateEnum::Menu, []() {
        return std::make_unique<MenuScreen>();
    });

    m_screenCache->registerScreen(StateEnum::Game, []() {
        return std::make_unique<GameScreen>();
    });

    m_screenCache->registerScreen(StateEnum::Exit, []() {
        return std::make_unique<ExitScreen>();
    });

}

void TerminalGameManager::render() {
    const StateEnum gameState = m_applicationGame->getGameState();
    IScreen& screen = m_screenCache->getScreen(gameState);

    screen.draw(*m_applicationGame);
}

bool TerminalGameManager::shouldExist() {
    return m_applicationGame->getGameState() == StateEnum::Exit;
}

void TerminalGameManager::input() {
    const StateEnum gameState = m_applicationGame->getGameState();
    IScreen& screen = m_screenCache->getScreen(gameState);

    screen.input(*m_applicationGame);
}

void TerminalGameManager::cleanup() {
    std::cout << "TerminalGameManager: cleanup" << std::endl;
}


