#pragma once

#include <memory>
#include <string>

#include "../../../8puzzle/engine/IScene.h"
#include "8puzzle/game/Record.h"

class RecordService;
class GamePlayService;
class GameOverScene final : public IScene {
    std::shared_ptr<GamePlayService> m_gamePlayService;
    std::shared_ptr<RecordService> m_recordService;
    std::string m_name;
    bool m_isRecord;

    [[nodiscard]] Record buildRecord() const;
    [[nodiscard]] Record buildRecordAndAssignRecord() const;

public:
    explicit GameOverScene(GameRouter& gameRouter);
    void onEnter() override;
    void draw() override;
    void input() override;
    void onExit() override;
};
