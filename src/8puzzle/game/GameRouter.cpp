#include "GameRouter.h"

#include <stdexcept>
#include <utility>

#include "state/StateGame.h"

GameRouter::GameRouter(std::shared_ptr<cengine::routing::ISceneRepository> sceneRepository)
    : m_sceneRepository(std::move(sceneRepository)) {
}

GameRouter::~GameRouter() = default;

StateGameFlow& GameRouter::castIt(cengine::routing::IState& state) {
    auto* flow = dynamic_cast<StateGameFlow*>(&state);
    if (!flow) {
        throw std::runtime_error("Estado atual não é do tipo StateGameFlow.");
    }
    return *flow;
}

void GameRouter::setNextState(std::unique_ptr<cengine::routing::IState> state) const {
    m_sceneRepository->persistNextState(std::move(state));
}

void GameRouter::introduction() {
    castIt(m_sceneRepository->getNextStateGame()).introduction(*this);
}

void GameRouter::menu() {
    castIt(m_sceneRepository->getNextStateGame()).menu(*this);
}

void GameRouter::game() {
    castIt(m_sceneRepository->getNextStateGame()).game(*this);
}

void GameRouter::gameOver() {
    castIt(m_sceneRepository->getNextStateGame()).gameOver(*this);
}

void GameRouter::record() {
    castIt(m_sceneRepository->getNextStateGame()).record(*this);
}

void GameRouter::exit() {
    castIt(m_sceneRepository->getNextStateGame()).exit(*this);
}
