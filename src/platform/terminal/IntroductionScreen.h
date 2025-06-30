#pragma once
#include "platform/IScreen.h"

class IntroductionScreen final : public IScreen {
public:
    explicit IntroductionScreen(GameRouter& gameRouter) : IScreen(gameRouter) {}

    void onEnter() override {}
    void draw() override;
    void input() override;
    void onExit() override {}
};
