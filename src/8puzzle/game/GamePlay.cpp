#include "GamePlay.h"

#include <chrono>
#include <iomanip>
#include <string>
#include <sstream>

GamePlay::GamePlay(): m_startedAt(std::chrono::system_clock::now()){}

void GamePlay::completeGame() {
    m_finishedAt = std::chrono::system_clock::now();
}

std::chrono::system_clock::time_point GamePlay::getStartedAt() const {
    return m_startedAt;
}

std::string GamePlay::getStartedAtAsString() const {
    return formatTimePoint(m_startedAt);
}

std::chrono::system_clock::time_point GamePlay::getFinishedAt() const {
    return m_finishedAt;
}

int GamePlay::getNumberOfMoves() const {
    return m_numberOfMoves;
}

void GamePlay::incrementMove() {
    m_numberOfMoves++;
}

long long GamePlay::getDurationMillis() const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        m_finishedAt - m_startedAt
    ).count();
}

std::string GamePlay::formatTimePoint(const std::chrono::system_clock::time_point& tp) {
    const std::time_t timeT = std::chrono::system_clock::to_time_t(tp);
    const std::tm localTm = *std::localtime(&timeT);  // Para horário local

    std::ostringstream oss;
    oss << std::put_time(&localTm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}