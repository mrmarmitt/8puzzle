#pragma once
#include <memory>

#include "../../../8puzzle/engine/IScene.h"

class GameRouter;
class InitialScene final : public IScene {
    std::shared_ptr<GameRouter> m_gameRouter;
public:
    explicit InitialScene();

    void onEnter() override {}
    void draw() override;
    void input() override {}
    void onExit() override {}
};
