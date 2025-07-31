#pragma once
#include <memory>
#include <string>

#include "../IState.h"
#include "repository/SceneRepository.h"

class IScene;
class IState;

class RouterService {
    std::shared_ptr<SceneRepository> m_sceneRepository;

public:
    explicit RouterService(std::shared_ptr<SceneRepository> sceneRepository);
    virtual ~RouterService() = default;

    void setNextState(std::unique_ptr<IState> state) const;

    [[nodiscard]] IState& getCurrentStateGame() const;
    [[nodiscard]] std::string getCurrentStateGameName() const;
    [[nodiscard]] std::string getCurrentStateGameCode() const;
    [[nodiscard]] IScene& getCurrentCachedScreen() const;

    [[nodiscard]] IState& getNextStateGame() const;
    [[nodiscard]] std::string getNextStateGameName() const;
    [[nodiscard]] std::string getNextStateGameCode() const;
    [[nodiscard]] IScene& getNextCachedScreen() const;

    [[nodiscard]] bool hasNextScreen() const;

    void goToNextScreen() const;


};
