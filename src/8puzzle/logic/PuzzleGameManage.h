#pragma once

#include "views/ViewState.h"

class PuzzleGameManage {
private:
    ViewState m_currentViewState;

public:
    PuzzleGameManage();

    ~PuzzleGameManage();
    ViewState getCurrentView();
    
    void updateCurrentView();
};