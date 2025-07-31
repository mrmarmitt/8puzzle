#pragma once

#include "../../../8puzzle/engine/IScene.h"

class ExitScene final : public IScene {

public:
    ExitScene() = default;

    void onEnter() override {}
    void draw() override;
    void input() override;
    void onExit() override {}
};
