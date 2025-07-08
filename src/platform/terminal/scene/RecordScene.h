#pragma once

#include <memory>

#include "../../../8puzzle/engine/IScene.h"

class RecordService;

enum class RecordSortType {
    ByTime,
    ByMoves
};

class RecordScene final : public IScene {
    RecordSortType m_sortType = RecordSortType::ByTime;
    std::shared_ptr<RecordService> m_recordService;

public:
    explicit RecordScene(GameRouter& gameRouter);

    void onEnter() override {}
    void draw() override;
    void input() override;
    void onExit() override {}

};
