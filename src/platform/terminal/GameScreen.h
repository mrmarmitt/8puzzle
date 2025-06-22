#pragma once
#include "8puzzle/core/Board.h"
#include "platform/IScreen.h"

class GameScreen final : public IScreen {
    Board m_board;
public:
    GameScreen() : m_board({
            Tile(1, "1"), Tile(2, "2"), Tile(3, "3"),
            Tile(4, "4"), Tile(5, "5"), Tile(6, "6"),
            Tile(7, "7"), Tile(0, " "), Tile(8, "8")
        })
    {   }

    void draw(ApplicationGame& applicationGame) override;
    void input(ApplicationGame& applicationGame) override;
};
