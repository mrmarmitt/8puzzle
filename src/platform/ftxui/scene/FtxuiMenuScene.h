#pragma once

#include <memory>

#include "FtxuiScene.h"

class GameRouter;
class GamePlayService;

// Cena do estado "menu": navegação por setas + ENTER (mesmas opções e mesma
// máquina de estados da versão terminal).
class FtxuiMenuScene final : public FtxuiScene {
    std::shared_ptr<GameRouter> m_gameRouter;
    std::shared_ptr<GamePlayService> m_gamePlayService;

    int m_selected = 0;

public:
    FtxuiMenuScene(std::shared_ptr<GameRouter> gameRouter,
                   std::shared_ptr<GamePlayService> gamePlayService);

    void draw() override;
    void input() override;
};
