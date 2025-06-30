#pragma once
#include "platform/IScreen.h"

class MenuScreen final : public IScreen {
    int m_selected = 0;

public:
    explicit MenuScreen(GameRouter& gameRouter) : IScreen(gameRouter) {}

    void onEnter() override {}
    void draw() override;
    void input() override;
    void onExit() override {}

};