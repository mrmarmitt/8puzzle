#pragma once
#include <memory>

#include "TerminalScene.h"

class GameRouter;
class IntroductionScene final : public TerminalScene {
    std::shared_ptr<GameRouter> m_gameRouter;
public:
    explicit IntroductionScene(std::shared_ptr<GameRouter> gameRouter);

    void onEnter() override {}
    void draw() override;
    void input() override;
    void onExit() override {}
};
