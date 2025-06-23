#pragma once
#include <string>

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

    const std::string& getName() const { return m_name; }
    int getNumberOfMoves() const { return m_numberOfMoves; }
    const std::string& getGamingAt() const { return m_gamingAt; }
    int getTime() const { return m_time; }

    bool isFasterThan(const Record& other) const {
        return m_time < other.m_time;
    }

    bool isSlowerThan(const Record& other) const {
        return m_time > other.m_time;
    }

    bool hasMoreMovesThan(const Record& other) const {
        return m_numberOfMoves > other.m_numberOfMoves;
    }

    bool hasLessMovesThan(const Record& other) const {
        return m_numberOfMoves < other.m_numberOfMoves;
    }
};
