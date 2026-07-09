#include "ForgeGameOverScene.h"

#include <utility>

#include "8puzzle/game/GameRouter.h"
#include "8puzzle/game/service/GamePlayService.h"
#include "8puzzle/game/service/RecordService.h"
#include "platform/Format.h"

ForgeGameOverScene::ForgeGameOverScene(std::shared_ptr<GameRouter> gameRouter,
                                       std::shared_ptr<GamePlayService> gamePlayService,
                                       std::shared_ptr<RecordService> recordService)
    : m_gameRouter(std::move(gameRouter)),
      m_gamePlayService(std::move(gamePlayService)),
      m_recordService(std::move(recordService)) {}

void ForgeGameOverScene::onEnter() {
    m_isRecord = m_recordService->isNewRecord(buildRecord(), 10);
}

void ForgeGameOverScene::draw() {
    const auto& gamePlay = m_gamePlayService->getGamePlay();
    const float h = forgeui::screenHeight();

    forgeui::drawTextCentered("FIM DE JOGO", h * 0.16f, 44.0f, forgeui::color::kTitle);
    forgeui::drawTextCentered("Voce resolveu o puzzle!", h * 0.16f + 64.0f, 26.0f, forgeui::color::kSuccess);

    const std::string stats = "Movimentos: " + std::to_string(gamePlay.getNumberOfMoves())
        + "     Tempo: " + formatMillis(gamePlay.getDurationMillis());
    forgeui::drawTextCentered(stats, h * 0.16f + 112.0f, 24.0f, forgeui::color::kValue);

    std::string hintText;
    if (m_isRecord) {
        forgeui::drawTextCentered("* NOVO RECORDE! *", h * 0.52f, 30.0f, forgeui::color::kValue);
        forgeui::drawTextCentered("Digite seu nome:", h * 0.52f + 48.0f, 22.0f, forgeui::color::kText);
        forgeui::drawTextCentered("[ " + m_name + "_ ]", h * 0.52f + 84.0f, 26.0f, forgeui::color::kAccent);
        hintText = "digite o nome   BACKSPACE apagar   ENTER confirmar";
    } else {
        forgeui::drawTextCentered("Sem recorde desta vez - tente de novo!", h * 0.55f, 22.0f, forgeui::color::kDim);
        hintText = "ENTER voltar ao menu";
    }

    forgeui::drawHints(hintText);
}

void ForgeGameOverScene::input() {
    const KeyEvent event = forgeui::readKey();

    if (!m_isRecord) {
        if (event.key == Key::Enter) {
            m_gameRouter->menu();
        }
        return;
    }

    switch (event.key) {
        case Key::Enter:
            m_recordService->addRecord(buildRecordAndAssignRecord());
            m_gameRouter->menu();
            break;
        case Key::Backspace:
            if (!m_name.empty()) {
                m_name.pop_back();
            }
            break;
        case Key::Char:
            if (m_name.size() < 20) {
                m_name.push_back(event.character);
            }
            break;
        default:
            break;
    }
}

Record ForgeGameOverScene::buildRecord() const {
    const auto& gamePlay = m_gamePlayService->getGamePlay();
    return {gamePlay.getNumberOfMoves(), gamePlay.getDurationMillis()};
}

Record ForgeGameOverScene::buildRecordAndAssignRecord() const {
    const auto& gamePlay = m_gamePlayService->getGamePlay();
    Record record{gamePlay.getNumberOfMoves(), gamePlay.getDurationMillis()};
    record.assignRecord(m_name.empty() ? "Anonimo" : m_name, gamePlay.getStartedAtAsString());
    return record;
}
