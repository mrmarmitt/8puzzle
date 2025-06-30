#pragma once

class IGameManager {
public:
    IGameManager() = default;
    virtual ~IGameManager() = default;

    virtual void onEnter() = 0;
    virtual void render() = 0;
    virtual void input() = 0;
    virtual void onExit() = 0;

    virtual bool shouldExist() = 0;
    virtual void cleanup() = 0;
};
