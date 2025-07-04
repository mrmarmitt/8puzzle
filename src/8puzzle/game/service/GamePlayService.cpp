#include "GamePlayService.h"

void GamePlayService::startNewGame() {
    m_currentGamePlay = std::make_shared<GamePlay>();
}

void GamePlayService::setCurrentGamePlay(const std::shared_ptr<GamePlay>& gamePlay) {
    m_currentGamePlay = gamePlay;
}

std::shared_ptr<GamePlay> GamePlayService::getCurrentGamePlay() const {
    return m_currentGamePlay;
}

void GamePlayService::clear() {
    m_currentGamePlay.reset();
    m_currentGamePlay = std::make_shared<GamePlay>();
}



