#pragma once

#include <memory>
#include <string>

#include "8puzzle/game/Record.h"
#include "FtxuiScene.h"

class GameRouter;
class GamePlayService;
class RecordService;

// Cena do estado "gameOver": resultado da partida; se entrou no top 10,
// captura o nome do jogador e registra o recorde (mesma regra do terminal).
class FtxuiGameOverScene final : public FtxuiScene {
    std::shared_ptr<GameRouter> m_gameRouter;
    std::shared_ptr<GamePlayService> m_gamePlayService;
    std::shared_ptr<RecordService> m_recordService;

    std::string m_name;
    bool m_isRecord = false;

    [[nodiscard]] Record buildRecord() const;
    [[nodiscard]] Record buildRecordAndAssignRecord() const;

public:
    FtxuiGameOverScene(std::shared_ptr<GameRouter> gameRouter,
                       std::shared_ptr<GamePlayService> gamePlayService,
                       std::shared_ptr<RecordService> recordService);

    void onEnter() override;
    void draw() override;
    void input() override;
};
