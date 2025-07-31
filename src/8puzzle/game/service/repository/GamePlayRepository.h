#pragma once

#include <memory>

#include "8puzzle/game/GamePlay.h"

class GamePlayRepository {
    std::unique_ptr<GamePlay> m_gamePlay;

public:
    GamePlayRepository() = default;
    ~GamePlayRepository() = default;

    void persist(std::unique_ptr<GamePlay> gamePlay);

    [[nodiscard]] GamePlay& get() const;

    void clear();
};
