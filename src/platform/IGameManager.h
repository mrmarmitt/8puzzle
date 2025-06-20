#pragma once

class IGameManager {
public:
    IGameManager() = default;
    virtual ~IGameManager() = default;

    virtual void input() = 0;
    virtual void render() = 0;
    virtual bool shouldExist() = 0;
    virtual void cleanup() = 0;
};
