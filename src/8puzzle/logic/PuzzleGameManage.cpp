#include "PuzzleGameManage.h"
#include "views/ViewState.h"


PuzzleGameManage::PuzzleGameManage() {
    m_currentViewState = ViewState::Intruduction;
}

PuzzleGameManage::~PuzzleGameManage() {
    m_currentViewState = ViewState::Intruduction;
}

ViewState PuzzleGameManage::getCurrentView() {
    return m_currentViewState;
}

void PuzzleGameManage::updateCurrentView() {

}

