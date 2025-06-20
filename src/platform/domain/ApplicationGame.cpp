#include "ApplicationGame.h"

#include "InitialSG.h"

ApplicationGame::ApplicationGame() {
    m_state = std::make_unique<InitialSG>();
}

void ApplicationGame::setState(std::unique_ptr<IStateGame> state) {
    m_state = std::move(state);
}

IStateGame& ApplicationGame::getState() const {
    return *m_state;
}

std::string ApplicationGame::getGameStateName() const {
    return m_state->getGameStateName();
}

GameState ApplicationGame::getGameState() const {
    return m_state->getGameState();
}

void ApplicationGame::introduction() {
    m_state->introduction(*this);
}

void ApplicationGame::menu() {
    m_state->menu(*this);
}

void ApplicationGame::exit() {
    m_state->exit(*this);
}
