#pragma once

#include <memory>

#include "ForgeScene.h"

class GameRouter;

// Cena do estado "introduction": como jogar; ENTER vai ao menu.
class ForgeInstructionsScene final : public ForgeScene {
    std::shared_ptr<GameRouter> m_gameRouter;

public:
    explicit ForgeInstructionsScene(std::shared_ptr<GameRouter> gameRouter);

    void draw() override;
    void input() override;
};
