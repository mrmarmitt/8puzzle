#pragma once

#include <memory>
#include <string>

#include "8puzzle/game/Record.h"
#include "ForgeScene.h"

class GameRouter;
class GamePlayService;
class RecordService;

// Cena do estado "gameOver": resumo da partida; se entrou no top-10, captura
// o nome e persiste o recorde (mesma logica da FtxuiGameOverScene).
class ForgeGameOverScene final : public ForgeScene {
    std::shared_ptr<GameRouter> m_gameRouter;
    std::shared_ptr<GamePlayService> m_gamePlayService;
    std::shared_ptr<RecordService> m_recordService;

    bool m_isRecord = false;
    std::string m_name;

    [[nodiscard]] Record buildRecord() const;
    [[nodiscard]] Record buildRecordAndAssignRecord() const;

public:
    ForgeGameOverScene(std::shared_ptr<GameRouter> gameRouter,
                       std::shared_ptr<GamePlayService> gamePlayService,
                       std::shared_ptr<RecordService> recordService);

    void onEnter() override;
    void draw() override;
    void input() override;
};
