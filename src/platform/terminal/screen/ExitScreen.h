#pragma once
#include "../../../8puzzle/engine/IScreen.h"

class ExitScreen final : public IScreen {
public:
    explicit ExitScreen(GameRouter& gameRouter) : IScreen(gameRouter) {}

    void onEnter() override {}
    void draw() override;
    void input() override;
    void onExit() override {}
};
