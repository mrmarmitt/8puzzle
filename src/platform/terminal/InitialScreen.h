#pragma once
#include "platform/IScreen.h"

class InitialScreen final : public IScreen {
public:
    InitialScreen() = default;
    void draw(ApplicationGame& applicationGame) override;
    void input(ApplicationGame& applicationGame) override;
};
