#pragma once

#include <memory>

#include "TerminalScene.h"

class GameRouter;
class GamePlayService;

class MenuScene final : public TerminalScene {
    std::shared_ptr<GameRouter> m_gameRouter;
    std::shared_ptr<GamePlayService> m_gamePlayService;

    int m_selected = 0;

public:
    explicit MenuScene(std::shared_ptr<GameRouter> gameRouter,
                       std::shared_ptr<GamePlayService> gamePlayService);

    void onEnter() override {
    }

    void draw() override;

    void input() override;

    void onExit() override {
    }
};
