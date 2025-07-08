#pragma once
#include "../../../8puzzle/engine/IScene.h"

class InitialScene final : public IScene {
public:
    explicit InitialScene(GameRouter& gameRouter) : IScene(gameRouter) {}

    void onEnter() override {}
    void draw() override;
    void input() override {}
    void onExit() override {}
};
