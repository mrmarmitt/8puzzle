#pragma once

#include <memory>

#include <cengine/core/Time.hpp>

#include "FtxuiScene.h"

class GameRouter;

// Cena do estado "initial": tabuleiro decorativo + convite piscante.
// O piscar usa o update(dt) da cengine 0.3.0 — primeira cena do projeto com
// lógica na fase de simulação.
class SplashScene final : public FtxuiScene {
    std::shared_ptr<GameRouter> m_gameRouter;
    cengine::core::Seconds m_elapsed{0};

public:
    explicit SplashScene(std::shared_ptr<GameRouter> gameRouter);

    void update(cengine::core::Seconds dt) override;
    void draw() override;
    void input() override;
};
