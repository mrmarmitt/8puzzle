#pragma once

#include <memory>

#include "GameManager.h"
#include "IWindowManager.h"

class EngineManager {
    void run();
    std::unique_ptr<IWindowManager> m_windowManager;
    std::unique_ptr<GameManager> m_gameManager;
    bool m_isRunning;

public:
    EngineManager(
        std::unique_ptr<IWindowManager> windowManager, 
        std::unique_ptr<GameManager> gameManager);

    ~EngineManager() = default;

    void start();
    void input() const;
    void cleanup() const;
};
