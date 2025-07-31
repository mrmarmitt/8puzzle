#pragma once

#include <memory>

#include "../../../8puzzle/engine/IScene.h"

class GameRouter;
class RecordService;

enum class RecordSortType {
    ByTime,
    ByMoves
};

class RecordScene final : public IScene {
    std::shared_ptr<GameRouter> m_gameRouter;
    std::shared_ptr<RecordService> m_recordService;
    RecordSortType m_sortType = RecordSortType::ByTime;

public:
    explicit RecordScene(std::shared_ptr<GameRouter> gameRouter, std::shared_ptr<RecordService> recordService);

    void onEnter() override {}
    void draw() override;
    void input() override;
    void onExit() override {}

};
