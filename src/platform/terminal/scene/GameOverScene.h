#pragma once

#include <memory>
#include <string>

#include <cengine/core/IScene.hpp>
#include "8puzzle/game/Record.h"
#include "8puzzle/game/service/GamePlayService.h"
#include "8puzzle/game/service/RecordService.h"

class GameRouter;

class GameOverScene final : public cengine::core::IScene {
    std::shared_ptr<GameRouter> m_gameRouter;
    std::shared_ptr<GamePlayService> m_gamePlayService;
    std::shared_ptr<RecordService> m_recordService;
    std::string m_name;
    bool m_isRecord;

    [[nodiscard]] Record buildRecord() const;

    [[nodiscard]] Record buildRecordAndAssignRecord() const;

public:
    explicit GameOverScene(
        std::shared_ptr<GameRouter> gameRouter,
        std::shared_ptr<GamePlayService> gamePlayService,
        std::shared_ptr<RecordService> recordService);

    void onEnter() override;

    void draw() override;

    void input() override;

    void onExit() override;
};
