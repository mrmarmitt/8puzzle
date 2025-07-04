#pragma once
#include "8puzzle/game/GameRouter.h"

class GameManager final {
    std::unique_ptr<IRouter> m_gameRouter;
public:
    explicit GameManager(IRouter& gameRouter) : m_gameRouter(&gameRouter) {}
    ~GameManager();

    void onEnter() const;
    void render() const;
    void input() const;
    void onExit() const;

    [[nodiscard]] bool shouldExist() const;
    static void cleanup();
};
