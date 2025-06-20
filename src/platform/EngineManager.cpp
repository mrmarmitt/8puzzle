#include "EngineManager.h"

#include <iostream>

#include "IGameManager.h"

EngineManager::EngineManager(std::unique_ptr<IWindowManager> windowManager,
                                 std::unique_ptr<IGameManager> gameManager,
                                 std::unique_ptr<FPSManager> fpsManager) :
        m_windowManager(std::move(windowManager)),
        m_gameManager(std::move(gameManager)),
        m_fpsManager(std::move(fpsManager)),
        m_isRunning(false) {
    }

// EngineManager::~EngineManager() {
//     cleanup();
// }

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
         m_gameManager->cleanup();
     }

     if (m_windowManager) {
         m_windowManager->cleanup();
     }

     if (m_fpsManager) {
         m_fpsManager->cleanup();
     }
}

void EngineManager::run() {
    while (m_isRunning) {
        if (m_fpsManager->shouldRender()) {
            m_windowManager->update();

            m_gameManager->input();
            m_gameManager->render();
            m_isRunning = !m_gameManager->shouldExist();
        }
    }
    cleanup();
}