#pragma once
#include "ViewState.h"


class IViewState {
public:
    virtual ~IViewState() = default;

    virtual ViewState getIntentionalView() = 0;
    virtual void cleanIntentionalView() = 0;
};