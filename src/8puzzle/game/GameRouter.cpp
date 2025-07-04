#include "GameRouter.h"

#include "state/StateGame.h"

GameRouter::GameRouter() {
    m_currentState = std::make_unique<InitialSG>();
    m_nextState = std::make_unique<InitialSG>();
}

GameRouter::~GameRouter() = default;

StateGameFlow& GameRouter::castIt(const std::unique_ptr<IState>& state) {
    auto* flow = dynamic_cast<StateGameFlow*>(state.get());
    if (!flow) {
        throw std::runtime_error("Estado atual não é do tipo StateGameFlow.");
    }
    return *flow;
}

void GameRouter::introduction() {
    castIt(m_nextState).introduction(*this);
}

void GameRouter::menu() {
    castIt(m_nextState).menu(*this);
}

void GameRouter::game() {
    castIt(m_nextState).game(*this);
}

void GameRouter::gameOver() {
    castIt(m_nextState).gameOver(*this);
}

void GameRouter::record() {
    castIt(m_nextState).record(*this);
}

void GameRouter::exit() {
    castIt(m_nextState).exit(*this);
}
