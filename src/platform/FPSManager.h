#pragma once
#include <chrono>

class FPSManager {
private:
    std::chrono::nanoseconds frameTimeNanos;
    std::chrono::steady_clock::time_point lastFrameTime;
    std::chrono::steady_clock::time_point lastFPSCheck;
    int frames;
    int currentFPS;

    void updateFrameStats(); // Privado: encapsula a l�gica de contagem

public:
    FPSManager(int targetFPS);
    ~FPSManager();

    // Retorna true se � hora de renderizar e j� atualiza internamente o contador
    bool shouldRender();

    int getCurrentFPS() const;

    void cleanup();
};