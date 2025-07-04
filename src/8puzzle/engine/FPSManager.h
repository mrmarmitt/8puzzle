#pragma once
#include <chrono>

class FPSManager {
    std::chrono::nanoseconds frameTimeNanos;
    std::chrono::steady_clock::time_point lastFrameTime;
    std::chrono::steady_clock::time_point lastFPSCheck;
    int frames;
    int currentFPS;

    void updateFrameStats();

public:
    explicit FPSManager(int targetFPS);
    ~FPSManager() = default;

    bool shouldRender();
    int getCurrentFPS() const;
    static void cleanup() ;
};