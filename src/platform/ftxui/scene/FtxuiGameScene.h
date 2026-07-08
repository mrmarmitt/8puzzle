#pragma once

#include <memory>

#include "8puzzle/game/core/Board.h"
#include "FtxuiScene.h"

class GameRouter;
class GamePlayService;

// Cena do estado "game": o puzzle jogável. O Board nasce embaralhado (200
// movimentos legais → sempre solucionável) a cada partida, já que a cena é
// recriada pela navegação. Movimenta por SETAS (a peça vizinha desliza para o
// vazio) ou pelos dígitos 1-8, como na versão terminal.
class FtxuiGameScene final : public FtxuiScene {
    std::shared_ptr<GameRouter> m_gameRouter;
    std::shared_ptr<GamePlayService> m_gamePlayService;

    Board m_board; // construtor default = tabuleiro embaralhado

    // Id da peça vizinha ao vazio na direção (dLine, dColumn); -1 se não há.
    [[nodiscard]] int neighborOfEmpty(int dLine, int dColumn) const;

    void tryMove(int tileId);

public:
    FtxuiGameScene(std::shared_ptr<GameRouter> gameRouter,
                   std::shared_ptr<GamePlayService> gamePlayService);

    void draw() override;
    void input() override;
};
