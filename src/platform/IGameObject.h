#pragma once
#include <functional>

class IGameObject{
public:
    virtual ~IGameObject() = default;

    virtual void input() = 0;
    virtual void render() = 0;
    virtual void cleanup() = 0;
};