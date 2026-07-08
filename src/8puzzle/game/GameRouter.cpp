#include "GameRouter.h"

#include <stdexcept>
#include <utility>

#include "state/StateGame.h"

GameRouter::GameRouter(std::shared_ptr<cengine::routing::IRouter> router)
    : m_router(std::move(router)) {
}

GameRouter::~GameRouter() = default;

const StateGameFlow& GameRouter::castIt(const cengine::routing::IState& state) {
    const auto* flow = dynamic_cast<const StateGameFlow*>(&state);
    if (!flow) {
        throw std::runtime_error("Estado atual não é do tipo StateGameFlow.");
    }
    return *flow;
}

void GameRouter::setNextState(std::unique_ptr<cengine::routing::IState> state) const {
    m_router->requestState(std::move(state));
}

// As transições despacham sobre o estado ATUAL do router (antes da 0.2.0 o
// despacho era sobre o "próximo" estado do repositório, que fora de uma troca
// pendente era uma cópia do atual — mesma semântica no fluxo do jogo).
void GameRouter::introduction() {
    castIt(m_router->currentState()).introduction(*this);
}

void GameRouter::menu() {
    castIt(m_router->currentState()).menu(*this);
}

void GameRouter::game() {
    castIt(m_router->currentState()).game(*this);
}

void GameRouter::gameOver() {
    castIt(m_router->currentState()).gameOver(*this);
}

void GameRouter::record() {
    castIt(m_router->currentState()).record(*this);
}

void GameRouter::exit() {
    castIt(m_router->currentState()).exit(*this);
}
