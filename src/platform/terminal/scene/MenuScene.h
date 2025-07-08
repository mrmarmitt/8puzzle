#pragma once

#include <memory>

#include "../../../8puzzle/engine/IScene.h"

class GamePlayService;
class MenuScene final : public IScene {
    std::shared_ptr<GamePlayService> m_gamePlayService;
    int m_selected = 0;

public:
    explicit MenuScene(GameRouter& gameRouter);

    void onEnter() override {}
    void draw() override;
    void input() override;
    void onExit() override {}

};