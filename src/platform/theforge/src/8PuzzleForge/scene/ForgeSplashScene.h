#pragma once

#include <memory>

#include <cengine/core/Time.hpp>

#include "ForgeScene.h"

class GameRouter;

// Cena do estado "initial": tabuleiro decorativo + convite piscante
// (mesmo comportamento da SplashScene FTXUI, dirigido pelo update(dt)).
class ForgeSplashScene final : public ForgeScene {
    std::shared_ptr<GameRouter> m_gameRouter;
    cengine::core::Seconds m_elapsed{0};

public:
    explicit ForgeSplashScene(std::shared_ptr<GameRouter> gameRouter);

    void update(cengine::core::Seconds dt) override;
    void draw() override;
    void input() override;
};
