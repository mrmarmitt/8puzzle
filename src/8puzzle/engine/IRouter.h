#pragma once
#include <memory>
#include <string>

#include "8puzzle/game/state/StateEnum.h"
// #include "8puzzle/game/state/StateGameOld.d"
#include "IState.h"
#include "SceneContainer.h"

class IState;
class IScene;
class SceneContainer;

class IRouter {

public:
    virtual ~IRouter() = default;

    std::unique_ptr<IState> m_nextState; //TODO: criar get/set e tornar privado
    std::unique_ptr<IState> m_currentState; //TODO: criar get/set e tornar privado

    virtual void setNextState(std::unique_ptr<IState> state) {
        m_nextState = std::move(state);
    }

    [[nodiscard]] virtual IState& getCurrentStateGame() const {
        return *m_currentState;
    }

    [[nodiscard]] virtual std::string getCurrentStateGameName() const {
        return m_currentState->getName();
    }

    [[nodiscard]] virtual std::string getCurrentStateGameCode() const {
        return m_currentState->getCode();
    }

    [[nodiscard]] virtual IScene& getCurrentCachedScreen() const {
        SceneContainer& container = SceneContainer::get();
        return container.getScene(m_currentState->getCode());
    }

    [[nodiscard]] virtual  bool hasNextScreen() const {
        return m_nextState->getCode() != m_currentState->getCode();
    }

    virtual void goToNextScreen() {
        if (m_nextState) {
            m_currentState = m_nextState->clone();
        } else {
            throw std::runtime_error("No next state set!");
        }
    }
};
