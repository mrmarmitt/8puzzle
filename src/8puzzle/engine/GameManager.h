#pragma once
#include <memory>

class GameRouter;
class GameManager final {
    std::shared_ptr<GameRouter> m_gameRouter;
public:
    explicit GameManager();
    ~GameManager();

    void onEnter() const;
    void render() const;
    void input() const;
    void onExit() const;

    [[nodiscard]] bool shouldExist() const;
    static void cleanup();
};
