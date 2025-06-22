#pragma once
#include "platform/IScreen.h"

class MenuScreen final : public IScreen {
    int m_selected = 0;

    static void gotoxy(int i, int i1);

public:
    MenuScreen() = default;

    void draw(ApplicationGame& applicationGame) override;
    void input(ApplicationGame& applicationGame) override;
};