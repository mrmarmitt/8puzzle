#pragma once
#include "platform/IScreen.h"

class ExitScreen final : public IScreen {
public:
    ExitScreen() = default;
    void draw(ApplicationGame& applicationGame) override;
    void input(ApplicationGame& applicationGame) override;
};
