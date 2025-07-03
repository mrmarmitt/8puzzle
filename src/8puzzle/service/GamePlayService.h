#pragma once
#include <memory>

#include "8puzzle/domain/GamePlay.h"

class GamePlayService {
    //repository .json para ser

    std::shared_ptr<GamePlay> m_currentGamePlay;

public:
    GamePlayService() = default;
    ~GamePlayService() = default;

    void startNewGame();
    void setCurrentGamePlay(const std::shared_ptr<GamePlay>& gamePlay);
    [[nodiscard]] std::shared_ptr<GamePlay> getCurrentGamePlay() const;
    // std::shared_ptr<GamePlay> getGamePlay();

    // [[nodiscard]] std::chrono::system_clock::time_point getStartedAt() const;
    // [[nodiscard]] std::chrono::system_clock::time_point getFinishedAt() const;
    // [[nodiscard]] int getNumberOfMoves() const;
    // void incrementMove() const;
    // void completeGame() const;

    void clear();

};
