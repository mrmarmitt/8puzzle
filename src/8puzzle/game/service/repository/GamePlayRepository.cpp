#include "GamePlayRepository.h"

void GamePlayRepository::persist(std::unique_ptr<GamePlay> gamePlay) {
    m_gamePlay = std::move(gamePlay);
}

GamePlay& GamePlayRepository::get() const {
    return *m_gamePlay;
}

void GamePlayRepository::clear()  {
    m_gamePlay.reset();
}


