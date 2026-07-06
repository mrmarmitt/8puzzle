#pragma once
#include <memory>

#include "TerminalScene.h"

class GameRouter;
class InitialScene final : public TerminalScene {
    std::shared_ptr<GameRouter> m_gameRouter;
public:
    explicit InitialScene(std::shared_ptr<GameRouter> gameRouter);

    void onEnter() override {}
    void draw() override;
    void input() override {}
    void onExit() override {}
};
