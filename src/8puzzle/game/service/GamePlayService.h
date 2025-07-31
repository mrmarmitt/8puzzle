#pragma once
#include <memory>

#include "8puzzle/game/GamePlay.h"

class GamePlayRepository;

class GamePlayService {
    const std::shared_ptr<GamePlayRepository> m_gamePlayRepository;

public:
    explicit GamePlayService(const std::shared_ptr<GamePlayRepository> &gamePlayRepository);

    ~GamePlayService() = default;

    void startNewGame() const;

    void setGamePlay(std::unique_ptr<GamePlay> gamePlay) const;

    [[nodiscard]] GamePlay &getGamePlay() const;

    void clear() const;
};
