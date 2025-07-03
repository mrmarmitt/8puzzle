#pragma once
#include <chrono>
#include <string>

class GamePlay {
    std::chrono::system_clock::time_point m_startedAt;
    std::chrono::system_clock::time_point m_finishedAt;
    int m_numberOfMoves = 0;

    static std::string formatTimePoint(const std::chrono::system_clock::time_point& tp);
public:
    GamePlay();
    ~GamePlay() = default;

    void completeGame();

    [[nodiscard]] std::chrono::system_clock::time_point getStartedAt() const;
    [[nodiscard]] std::string getStartedAtAsString() const;
    [[nodiscard]] std::chrono::system_clock::time_point getFinishedAt() const;
    [[nodiscard]] int getNumberOfMoves() const;
    void incrementMove();
    [[nodiscard]] long long getDurationMillis() const;
};
