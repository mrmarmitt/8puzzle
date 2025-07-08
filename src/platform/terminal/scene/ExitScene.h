#pragma once
#include "../../../8puzzle/engine/IScene.h"

class ExitScene final : public IScene {
public:
    explicit ExitScene(GameRouter& gameRouter) : IScene(gameRouter) {}

    void onEnter() override {}
    void draw() override;
    void input() override;
    void onExit() override {}
};
