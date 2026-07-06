#pragma once

#include "TerminalScene.h"

class ExitScene final : public TerminalScene {

public:
    ExitScene() = default;

    void onEnter() override {}
    void draw() override;
    void input() override;
    void onExit() override {}
};
