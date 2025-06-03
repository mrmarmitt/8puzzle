#pragma once

#include "IWindowManager.h"
#include "IGameStateManager.h"
#include "FPSManager.h"

class EngineManager {
private:
    void run();
    std::unique_ptr <IWindowManager> m_windowManager;
    std::unique_ptr <IGameStateManager> m_gameStateManager;
    std::unique_ptr<FPSManager> m_fpsManager;
    bool m_isRunning;

public:
    EngineManager(
        std::unique_ptr<IWindowManager> windowManager, 
        std::unique_ptr<IGameStateManager> gameStateManager,
        std::unique_ptr<FPSManager> fpsManager);

    ~EngineManager();

    void start();
    void input();
    void cleanup();
};
