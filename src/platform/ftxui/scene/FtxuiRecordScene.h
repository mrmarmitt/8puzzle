#pragma once

#include <memory>

#include "FtxuiScene.h"

class GameRouter;
class RecordService;

enum class FtxuiRecordSort {
    ByTime,
    ByMoves
};

// Cena do estado "record": tabela de recordes ordenável por tempo (T) ou
// movimentos (M), como na versão terminal.
class FtxuiRecordScene final : public FtxuiScene {
    std::shared_ptr<GameRouter> m_gameRouter;
    std::shared_ptr<RecordService> m_recordService;
    FtxuiRecordSort m_sortType = FtxuiRecordSort::ByTime;

public:
    FtxuiRecordScene(std::shared_ptr<GameRouter> gameRouter,
                     std::shared_ptr<RecordService> recordService);

    void draw() override;
    void input() override;
};
