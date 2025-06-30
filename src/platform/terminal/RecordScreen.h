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

public:
    explicit RecordScreen(GameRouter& gameRouter) :
        IScreen(gameRouter) {}

    void onEnter() override {}
    void draw() override;
    void input() override;
    void onExit() override {}

};
