#pragma once

#include <memory>

#include "FtxuiScene.h"

class GameRouter;

// Cena do estado "introduction": como jogar, antes do menu.
class InstructionsScene final : public FtxuiScene {
    std::shared_ptr<GameRouter> m_gameRouter;

public:
    explicit InstructionsScene(std::shared_ptr<GameRouter> gameRouter);

    void draw() override;
    void input() override;
};
