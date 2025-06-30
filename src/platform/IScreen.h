#pragma once

class GamePlay;
class GameRouter;

class IScreen {

    GameRouter& m_gameRouter;
    // GamePlay& m_gamePlay;

    bool isEnterExecuted = false;
    bool isExitExecuted = false;
public:
    explicit IScreen(GameRouter& gameRouter/*, GamePlay& gamePlay*/) :
        m_gameRouter(gameRouter)/*, m_gamePlay(gamePlay)*/{}

    virtual ~IScreen() = default;

    [[nodiscard]] GameRouter& getGameRouter() const { return m_gameRouter;}

    virtual void onEnter() = 0;
    virtual void onEnterExecuted() { isEnterExecuted = true;}
    virtual void draw() = 0;
    virtual void input() = 0;
    virtual void onExit() = 0;
    virtual void onExitExecuted() { isExitExecuted = true;}
};
