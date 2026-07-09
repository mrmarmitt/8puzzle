#pragma once

#include "ForgeScene.h"

// Cena do estado "exit" (kExitStateCode): despedida — o adaptador ve
// shouldExit() e pede o shutdown do framework; ela desenha no maximo um
// ou dois quadros.
class ForgeExitScene final : public ForgeScene {
public:
    ForgeExitScene() = default;

    void draw() override;
    void input() override {}
};
