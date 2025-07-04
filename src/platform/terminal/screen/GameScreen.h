#pragma once

// #include <chrono>
#include <memory>

#include "../../../8puzzle/engine/IScreen.h"
#include "8puzzle/core/Board.h"

class GamePlayService;
class GameScreen final : public IScreen {
    std::shared_ptr<GamePlayService> m_gamePlayService;

    // Board m_board;
    Board m_board = Board({
        Tile(1, "1"), Tile(2, "2"), Tile(3, "3"),
        Tile(4, "4"), Tile(5, "5"), Tile(6, "6"),
        Tile(7, "7"), Tile(0, " "), Tile(8, "8")
    });
    [[nodiscard]] std::string printBoard() const;

public:

    explicit GameScreen(GameRouter& gameRouter);

    void onEnter() override {}
    void draw() override;
    void input() override;
    void onExit() override;
};
