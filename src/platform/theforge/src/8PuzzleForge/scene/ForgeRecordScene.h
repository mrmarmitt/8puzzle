#pragma once

#include <memory>

#include "ForgeScene.h"

class GameRouter;
class RecordService;

enum class ForgeRecordSort { ByTime, ByMoves };

// Cena do estado "record": top recordes com ordenacao alternavel (T/M),
// mesma logica da FtxuiRecordScene.
class ForgeRecordScene final : public ForgeScene {
    std::shared_ptr<GameRouter> m_gameRouter;
    std::shared_ptr<RecordService> m_recordService;

    ForgeRecordSort m_sortType = ForgeRecordSort::ByTime;

public:
    ForgeRecordScene(std::shared_ptr<GameRouter> gameRouter,
                     std::shared_ptr<RecordService> recordService);

    void draw() override;
    void input() override;
};
