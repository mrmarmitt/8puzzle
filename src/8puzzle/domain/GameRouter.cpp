#include "GameRouter.h"

#include "platform/ScreenContainer.h"
#include "state/StateEnum.h"
#include "state/StateGame.h"

GameRouter::GameRouter() {
    m_currentState = std::make_unique<InitialSG>();
    m_nextState = std::make_unique<InitialSG>();
}

GameRouter::~GameRouter() = default;

void GameRouter::setNextState(std::unique_ptr<IStateGame> state) {
    m_nextState = std::move(state);
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
bool GameRouter::hasNextScreen() const {
    return m_nextState->getGameStateEnum() != m_currentState->getGameStateEnum();
}

void GameRouter::goToNextScreen() {
    if (m_nextState) {
        m_currentState = m_nextState->clone();
    } else {
        throw std::runtime_error("No next state set!");
    }
}

void GameRouter::introduction() {
    m_nextState->introduction(*this);
}

void GameRouter::menu() {
    m_nextState->menu(*this);
}

void GameRouter::game() {
    m_nextState->game(*this);
}

void GameRouter::gameOver() {
    m_nextState->gameOver(*this);
}

void GameRouter::record() {
    m_nextState->record(*this);
}

void GameRouter::exit() {
    m_nextState->exit(*this);
}
