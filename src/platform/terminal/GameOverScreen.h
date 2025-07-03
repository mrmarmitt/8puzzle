#pragma once

#include <memory>
#include <string>

#include "8puzzle/domain/Record.h"
#include "platform/IScreen.h"

class RecordService;
class GamePlayService;
class GameOverScreen final : public IScreen {
    std::shared_ptr<GamePlayService> m_gamePlayService;
    std::shared_ptr<RecordService> m_recordService;
    std::string m_name;
    bool m_isRecord;

    [[nodiscard]] Record buildRecord() const;
    [[nodiscard]] Record buildRecordAndAssignRecord() const;

public:
    explicit GameOverScreen(GameRouter& gameRouter);
    void onEnter() override;
    void draw() override;
    void input() override;
    void onExit() override;
};
