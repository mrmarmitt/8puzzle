#pragma once

#include <memory>

#include "platform/IScreen.h"

class RecordService;

enum class RecordSortType {
    ByTime,
    ByMoves
};

class RecordScreen final : public IScreen {
    RecordSortType m_sortType = RecordSortType::ByTime;
    std::shared_ptr<RecordService> m_recordService;

public:
    explicit RecordScreen(GameRouter& gameRouter);

    void onEnter() override {}
    void draw() override;
    void input() override;
    void onExit() override {}

};
