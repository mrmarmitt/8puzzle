#pragma once
#include "domain/ApplicationGame.h"

class IScreen {
public:
    IScreen() = default;
    virtual ~IScreen() = default;

    virtual void draw(ApplicationGame& applicationGame) = 0;
    virtual void input(ApplicationGame& applicationGame) = 0;
};
