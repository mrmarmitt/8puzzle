#include "GamePlayService.h"

#include "repository/GamePlayRepository.h"

GamePlayService::GamePlayService(const std::shared_ptr<GamePlayRepository> &gamePlayRepository) : m_gamePlayRepository(gamePlayRepository) {
}

void GamePlayService::startNewGame() const {
    m_gamePlayRepository->persist(std::make_unique<GamePlay>());
}

void GamePlayService::setGamePlay(std::unique_ptr<GamePlay> gamePlay) const {
    m_gamePlayRepository->persist(std::move(gamePlay));
}

GamePlay &GamePlayService::getGamePlay() const {
    return m_gamePlayRepository->get();
}

void GamePlayService::clear() const {
    m_gamePlayRepository->clear();
}
