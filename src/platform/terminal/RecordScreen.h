#pragma once
#include "platform/IScreen.h"

class RecordService;

enum class RecordSortType {
    ByTime,
    ByMoves
};

class RecordScreen final : public IScreen {

    std::unique_ptr<RecordService> m_recordService;
    RecordSortType m_sortType = RecordSortType::ByTime;

public:
    explicit RecordScreen(RecordService& service);

    void draw(ApplicationGame& applicationGame) override;
    void input(ApplicationGame& applicationGame) override;
};
