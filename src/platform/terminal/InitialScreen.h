#pragma once
#include "platform/IScreen.h"

class InitialScreen final : public IScreen {
public:
    explicit InitialScreen(GameRouter& gameRouter) : IScreen(gameRouter) {}

    void onEnter() override {}
    void draw() override;
    void input() override {}
    void onExit() override {}
};
