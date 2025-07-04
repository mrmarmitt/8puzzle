#include "FPSManager.h"

#include <iostream>

using namespace std::chrono;

FPSManager::FPSManager(int targetFPS) :
    frameTimeNanos(1'000'000'000 / targetFPS),
    lastFrameTime(steady_clock::now()),
    lastFPSCheck(steady_clock::now()),
    frames(0),
    currentFPS(targetFPS) {
}

bool FPSManager::shouldRender() {
    const auto now = steady_clock::now();
    auto elapsed = now - lastFrameTime;

    if (elapsed >= frameTimeNanos) {
        lastFrameTime = now;
        updateFrameStats(); // Atualiza internamente
        return true;
    }
    return false;
}

void FPSManager::updateFrameStats() {
    auto now = steady_clock::now();
    frames++;

    if (duration_cast<milliseconds>(now - lastFPSCheck).count() >= 1000) {
        currentFPS = frames;
        frames = 0;
        lastFPSCheck = now;
    }
}

int FPSManager::getCurrentFPS() const {
    return currentFPS;
}

void FPSManager::cleanup() {
    std::cout << "FPSManager: cleanup" << std::endl;
}
