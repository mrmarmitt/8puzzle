#pragma once

#include <memory>

#include "8puzzle/game/core/Board.h"
#include "ForgeScene.h"

class GameRouter;
class GamePlayService;

// Cena do estado "game": o puzzle jogavel. O Board nasce embaralhado (200
// movimentos legais -> sempre solucionavel) a cada partida, ja que a cena e
// recriada pela navegacao. Movimenta por SETAS (a peca vizinha desliza para o
// vazio) ou pelos digitos 1-8 — mesma logica da FtxuiGameScene.
class ForgeGameScene final : public ForgeScene {
    std::shared_ptr<GameRouter> m_gameRouter;
    std::shared_ptr<GamePlayService> m_gamePlayService;

    Board m_board; // construtor default = tabuleiro embaralhado

    // Id da peca vizinha ao vazio na direcao (dLine, dColumn); -1 se nao ha.
    [[nodiscard]] int neighborOfEmpty(int dLine, int dColumn) const;

    void tryMove(int tileId);

public:
    ForgeGameScene(std::shared_ptr<GameRouter> gameRouter,
                   std::shared_ptr<GamePlayService> gamePlayService);

    void draw() override;
    void input() override;
};
