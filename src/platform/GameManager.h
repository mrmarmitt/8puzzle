#pragma once
#include "8puzzle/domain/GameRouter.h"

class GameManager final {
    std::unique_ptr<GameRouter> m_gameRouter;
public:
    explicit GameManager(GameRouter& gameRouter) : m_gameRouter(&gameRouter) {}
    ~GameManager();

    void onEnter() const;
    void render() const;
    void input() const;
    void onExit() const;

    [[nodiscard]] bool shouldExist() const;
    static void cleanup();
};
