#pragma once
#include <string>
#include <utility>

class Record {
    std::string m_name;
    int m_numberOfMoves = 0;
    std::string m_gamingAt;
    long long m_durationMillis = 0;

public:
    Record() = default;

    Record(std::string name, const int moves, std::string gamingAt, const long long durationMillis)
    : m_name(std::move(name)), m_numberOfMoves(moves),
      m_gamingAt(std::move(gamingAt)), m_durationMillis(durationMillis) {}

    Record(const int moves, const long long durationMillis) : m_numberOfMoves(moves), m_durationMillis(durationMillis) {}

    void assignRecord(std::string name, std::string gamingAt) {
        m_name = std::move(name);
        m_gamingAt = std::move(gamingAt);
    }

    [[nodiscard]] const std::string& getName() const { return m_name; }
    [[nodiscard]] int getNumberOfMoves() const { return m_numberOfMoves; }
    [[nodiscard]] const std::string& getGamingAt() const { return m_gamingAt; }
    [[nodiscard]] long long getTime() const { return m_durationMillis; }

    [[nodiscard]] bool isFasterThan(const Record& other) const {
        return m_durationMillis < other.m_durationMillis;
    }

    [[nodiscard]] bool isSlowerThan(const Record& other) const {
        return m_durationMillis > other.m_durationMillis;
    }

    [[nodiscard]] bool hasMoreMovesThan(const Record& other) const {
        return m_numberOfMoves > other.m_numberOfMoves;
    }

    [[nodiscard]] bool hasLessMovesThan(const Record& other) const {
        return m_numberOfMoves < other.m_numberOfMoves;
    }
};
