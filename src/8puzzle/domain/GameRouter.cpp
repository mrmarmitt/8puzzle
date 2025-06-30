#include "GameRouter.h"

#include "platform/ScreenContainer.h"
#include "state/StateEnum.h"
#include "state/StateGame.h"


GameRouter::GameRouter() {
    m_currentState = std::make_unique<InitialSG>();
}

GameRouter::~GameRouter() = default;

void GameRouter::setState(std::unique_ptr<IStateGame> state) {
    m_currentState = std::move(state);
}

IStateGame& GameRouter::getCurrentStateGame() const {
    return *m_currentState;
}

std::string GameRouter::getCurrentStateGameName() const {
    return m_currentState->getGameStateName();
}
StateEnum GameRouter::getCurrentStateGameEnum() const {
    return m_currentState->getGameStateEnum();
}

IScreen& GameRouter::getCurrentCachedScreen() const {
    ScreenContainer& container = ScreenContainer::get();
    return container.getScreen(m_currentState->getGameStateEnum());
}

void GameRouter::introduction() {
    m_currentState->introduction(*this);
}

void GameRouter::menu() {
    m_currentState->menu(*this);
}

void GameRouter::game() {
    m_currentState->game(*this);
}

void GameRouter::gameOver() {
    m_currentState->gameOver(*this);
}

void GameRouter::record() {
    m_currentState->record(*this);
}

void GameRouter::exit() {
    m_currentState->exit(*this);
}
