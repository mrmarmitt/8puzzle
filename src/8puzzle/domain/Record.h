#pragma once
#include <string>
#include <utility>

class Record {
    std::string m_name;
    int m_numberOfMoves = 0;
    std::string m_gamingAt;
    int m_time = 0;

public:
    Record() = default;

    Record(std::string name, const int moves, std::string gamingAt, const int time)
    : m_name(std::move(name)), m_numberOfMoves(moves),
      m_gamingAt(std::move(gamingAt)), m_time(time) {}

    Record(const int moves, const int time) : m_numberOfMoves(moves), m_time(time) {}

    void assignRecord(std::string name, std::string gamingAt) {
        m_name = std::move(name);
        m_gamingAt = std::move(gamingAt);
    }

    [[nodiscard]] const std::string& getName() const { return m_name; }
    [[nodiscard]] int getNumberOfMoves() const { return m_numberOfMoves; }
    [[nodiscard]] const std::string& getGamingAt() const { return m_gamingAt; }
    [[nodiscard]]  int getTime() const { return m_time; }

    [[nodiscard]] bool isFasterThan(const Record& other) const {
        return m_time < other.m_time;
    }

    [[nodiscard]] bool isSlowerThan(const Record& other) const {
        return m_time > other.m_time;
    }

    [[nodiscard]] bool hasMoreMovesThan(const Record& other) const {
        return m_numberOfMoves > other.m_numberOfMoves;
    }

    bool hasLessMovesThan(const Record& other) const {
        return m_numberOfMoves < other.m_numberOfMoves;
    }
};
