#pragma once

#include <unordered_map>
#include "IGameObject.h"
//#include "GameState.cpp"
#include <memory>

class IGameStateManager{

public:
    //virtual void addGameObject(GameState status, std::unique_ptr<IGameObject> object) = 0;
    //virtual void setCurrentState(GameState gameState) = 0;
    virtual void input() = 0;
    virtual void render() = 0;
    virtual void cleanup() = 0;
};