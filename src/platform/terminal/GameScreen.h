#pragma once
#include <chrono>

#include "8puzzle/core/Board.h"
#include "8puzzle/service/RecordService.h"
#include "platform/IScreen.h"

class GameScreen final : public IScreen {
    Board m_board;
    std::unique_ptr<RecordService> m_recordService;
    int m_moves = 0;
    int m_time = 0;

    std::chrono::steady_clock::time_point m_startTime;
    bool m_started = false;


public:
    explicit GameScreen(RecordService& service);
    void draw(ApplicationGame& applicationGame) override;
    void input(ApplicationGame& applicationGame) override;
    [[nodiscard]] std::string printBoard();
};
