#include "EngineManager.h"
#include "IGameStateManager.h"

EngineManager::EngineManager(std::unique_ptr<IWindowManager> windowManager,
                             std::unique_ptr<IGameStateManager> gameStateManager,
                             std::unique_ptr<FPSManager> fpsManager) :
    m_windowManager(std::move(windowManager)),
    m_gameStateManager(std::move(gameStateManager)),
    m_fpsManager(std::move(fpsManager)),
    m_isRunning(false) {
}

EngineManager::~EngineManager() {
    cleanup();
}

void EngineManager::start() {
    if (m_windowManager) {
        m_windowManager->init();
    }

    m_isRunning = true;
    run();
}

void EngineManager::input() {
    if (m_gameStateManager) {
        m_gameStateManager->input();
    }
}

void EngineManager::cleanup() {
    if (m_gameStateManager) {
        m_gameStateManager->cleanup();
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

            m_gameStateManager->input();
            m_gameStateManager->render();
        }
    }
    cleanup();
}