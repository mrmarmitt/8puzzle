#pragma once

#include <memory>

#include "platform/IScreen.h"

class GamePlayService;
class MenuScreen final : public IScreen {
    std::shared_ptr<GamePlayService> m_gamePlayService;
    int m_selected = 0;

public:
    explicit MenuScreen(GameRouter& gameRouter);

    void onEnter() override {}
    void draw() override;
    void input() override;
    void onExit() override {}

};