#include "EngineManager.h"

#include <iostream>

EngineManager::EngineManager(std::unique_ptr<IWindowManager> windowManager,
                             std::unique_ptr<GameManager> gameManager) : m_windowManager(std::move(windowManager)),
                                                                         m_gameManager(std::move(gameManager)),
                                                                         m_isRunning(false) {
}

void EngineManager::start() {
    if (m_windowManager) {
        m_windowManager->init();
    }

    m_isRunning = true;
    run();
}

void EngineManager::input() const {
    if (m_gameManager) {
        m_gameManager->input();
    }
}

void EngineManager::cleanup() const {
    std::cout << "EngineManager: cleanup" << std::endl;

    if (m_gameManager) {
        GameManager::cleanup();
    }

    if (m_windowManager) {
        m_windowManager->cleanup();
    }
}

void EngineManager::run() {
    while (m_isRunning) {
        m_windowManager->update();

        m_gameManager->onEnter();
        m_gameManager->input();
        m_gameManager->render();
        m_gameManager->onExit();

        m_isRunning = !m_gameManager->shouldExist();
    }
    cleanup();
}
