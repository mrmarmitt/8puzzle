#pragma once
#include "platform/IScreen.h"

class MenuScreen final : public IScreen {
    int m_selected = 0;

public:
    MenuScreen() = default;

    void draw(ApplicationGame& applicationGame) override;
    void input(ApplicationGame& applicationGame) override;
};