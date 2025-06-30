#pragma once
#include <chrono>

#include "8puzzle/core/Board.h"
#include "platform/IScreen.h"

class GameScreen final : public IScreen {
    Board m_board = Board({
        Tile(1, "1"), Tile(2, "2"), Tile(3, "3"),
        Tile(4, "4"), Tile(5, "5"), Tile(6, "6"),
        Tile(7, "7"), Tile(0, " "), Tile(8, "8")
    });

    int m_moves = 0;
    int m_time = 0;

    std::chrono::steady_clock::time_point m_startTime;
    bool m_started = false;

    [[nodiscard]] std::string printBoard();
public:

    explicit GameScreen(GameRouter& gameRouter) : IScreen(gameRouter) {}

    void onEnter() override {}
    void draw() override;
    void input() override;
    void onExit() override {}


};
