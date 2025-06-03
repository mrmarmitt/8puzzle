#pragma once
#include "IViewState.h"

class Credit : public IViewState {
public:
    Credit();
    ~Credit();

    ViewState getIntentionalView() override;
    void cleanIntentionalView() override;

};