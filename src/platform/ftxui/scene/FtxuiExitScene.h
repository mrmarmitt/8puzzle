#pragma once

#include "FtxuiScene.h"

// Cena do estado "exit" (kExitStateCode): despedida; o loop encerra sozinho.
class FtxuiExitScene final : public FtxuiScene {
public:
    FtxuiExitScene() = default;

    void draw() override;
    void input() override {}
};
