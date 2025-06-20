#pragma once
#include "platform/IScreen.h"

class MenuScreen final : public IScreen {
public:
    MenuScreen() = default;
    void draw(ApplicationGame& applicationGame) override;
    void input(ApplicationGame& applicationGame) override;
};