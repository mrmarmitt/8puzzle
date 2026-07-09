#pragma once

#include <memory>

#include "ForgeScene.h"

class GameRouter;
class GamePlayService;

// Cena do estado "menu": navegacao por setas + ENTER, como a FtxuiMenuScene.
class ForgeMenuScene final : public ForgeScene {
    std::shared_ptr<GameRouter> m_gameRouter;
    std::shared_ptr<GamePlayService> m_gamePlayService;

    int m_selected = 0;

public:
    ForgeMenuScene(std::shared_ptr<GameRouter> gameRouter,
                   std::shared_ptr<GamePlayService> gamePlayService);

    void draw() override;
    void input() override;
};
