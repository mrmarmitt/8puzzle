#pragma once
#include "../../../8puzzle/engine/IScene.h"

class IntroductionScene final : public IScene {
public:
    explicit IntroductionScene(GameRouter& gameRouter) : IScene(gameRouter) {}

    void onEnter() override {}
    void draw() override;
    void input() override;
    void onExit() override {}
};
