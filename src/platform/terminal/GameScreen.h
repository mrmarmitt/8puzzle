#pragma once
#include "8puzzle/core/Board.h"
#include "platform/IScreen.h"

class GameScreen final : public IScreen {
    Board m_board;

public:
    GameScreen() = default;
    void draw(ApplicationGame& applicationGame) override;
    void input(ApplicationGame& applicationGame) override;
    [[nodiscard]] std::string printBoard() const;
};
