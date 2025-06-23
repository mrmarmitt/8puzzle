#include "ApplicationGame.h"

#include "state/StateGame.h"

ApplicationGame::ApplicationGame() {
    m_state = std::make_unique<InitialSG>();
}

ApplicationGame::~ApplicationGame() = default;

void ApplicationGame::setState(std::unique_ptr<IStateGame> state) {
    m_state = std::move(state);
}

IStateGame& ApplicationGame::getState() const {
    return *m_state;
}

std::string ApplicationGame::getGameStateName() const {
    return m_state->getGameStateName();
}

StateEnum ApplicationGame::getGameState() const {
    return m_state->getGameState();
}

void ApplicationGame::introduction() {
    m_state->introduction(*this);
}

void ApplicationGame::menu() {
    m_state->menu(*this);
}

void ApplicationGame::game() {
    m_state->game(*this);
}
void ApplicationGame::record() {
    m_state->record(*this);
}

void ApplicationGame::exit() {
    m_state->exit(*this);
}
