#pragma once

class GamePlay;
class GameRouter;

class IScreen {

    GameRouter& m_gameRouter;

    bool isEnterExecuted = false;

public:
    explicit IScreen(GameRouter& gameRouter) :
        m_gameRouter(gameRouter){}

    virtual ~IScreen() = default;

    [[nodiscard]] GameRouter& getGameRouter() const { return m_gameRouter;}

    virtual void onEnter() = 0;
    virtual void onEnterExecuted() { isEnterExecuted = true;}
    [[nodiscard]] virtual bool isOnEnterExecuted() const { return isEnterExecuted;}
    virtual void draw() = 0;
    virtual void input() = 0;
    virtual void onExit() = 0;
    virtual void resetConst() { isEnterExecuted = false; }

};
