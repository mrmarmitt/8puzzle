#pragma once

#include <memory>

#include "8puzzle/service/RecordService.h"
#include "platform/IScreen.h"

class GameOverScreen final : public IScreen {
    // std::unique_ptr<RecordService> m_recordService;
    std::string m_name;
    bool m_isRecord = false;

public:
    // explicit GameOverScreen(RecordService& recordService);

    explicit GameOverScreen(GameRouter& gameRouter) : IScreen(gameRouter) {}
    void onEnter() override;
    void draw() override;
    void input() override;
    void onExit() override;
};
