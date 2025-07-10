#pragma once

#include <memory>

#include "FPSManager.h"
#include "GameManager.h"
#include "IWindowManager.h"

class EngineManager {
    void run();
    std::unique_ptr<IWindowManager> m_windowManager;
    std::unique_ptr<GameManager> m_gameManager;
    std::unique_ptr<FPSManager> m_fpsManager;
    bool m_isRunning;

public:
    EngineManager(
        std::unique_ptr<IWindowManager> windowManager, 
        std::unique_ptr<GameManager> gameManager,
        std::unique_ptr<FPSManager> fpsManager);

    ~EngineManager() = default;

    void start();
    void input() const;
    void cleanup() const;
};
