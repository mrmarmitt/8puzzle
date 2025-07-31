#include "GameRouter.h"

#include <utility>

#include "state/StateGame.h"

GameRouter::GameRouter(std::shared_ptr<SceneRepository> sceneRepository) : RouterService(std::move(sceneRepository)) {
}

GameRouter::~GameRouter() = default;

StateGameFlow& GameRouter::castIt(IState& state) {
    auto* flow = dynamic_cast<StateGameFlow*>(&state);
    if (!flow) {
        throw std::runtime_error("Estado atual não é do tipo StateGameFlow.");
    }
    return *flow;
}

void GameRouter::introduction() {
    castIt(getNextStateGame()).introduction(*this);
}

void GameRouter::menu() {
    castIt(getNextStateGame()).menu(*this);
}

void GameRouter::game() {
    castIt(getNextStateGame()).game(*this);
}

void GameRouter::gameOver() {
    castIt(getNextStateGame()).gameOver(*this);
}

void GameRouter::record() {
    castIt(getNextStateGame()).record(*this);
}

void GameRouter::exit() {
    castIt(getNextStateGame()).exit(*this);
}
