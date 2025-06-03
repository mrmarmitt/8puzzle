#pragma once

#include <unordered_map>

#include "ViewState.h"
#include "IViewState.h"

#include "Credit.h"

class ViewStateContainer {
private:
    std::unordered_map<ViewState, IViewState*> viewStates = {
        //{ViewState::Intruduction, nullptr}, 
        {ViewState::Credit, new Credit()}
        //{ViewState::MainMenu, nullptr},     
        //{ViewState::LoadGame, nullptr},
        //{ViewState::Records, nullptr},      
        //{ViewState::Settings, nullptr},
        //{ViewState::Game, nullptr},         
        //{ViewState::GameOver, nullptr},
        //{ViewState::Pause, nullptr},        
        //{ViewState::Exit, nullptr}
    };

public:
    void getView();
};