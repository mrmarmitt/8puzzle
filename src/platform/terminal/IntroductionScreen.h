#pragma once
#include "platform/IScreen.h"

class IntroductionScreen final : public IScreen {
public:
    IntroductionScreen() = default;
    void draw(ApplicationGame& applicationGame) override;
    void input(ApplicationGame& applicationGame) override;
};
