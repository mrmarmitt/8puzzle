#include "FtxuiGameOverScene.h"

#include <utility>

#include "8puzzle/game/GameRouter.h"
#include "8puzzle/game/service/GamePlayService.h"
#include "8puzzle/game/service/RecordService.h"
#include "../Format.h"
#include "../Keyboard.h"

using namespace ftxui;

FtxuiGameOverScene::FtxuiGameOverScene(std::shared_ptr<GameRouter> gameRouter,
                                       std::shared_ptr<GamePlayService> gamePlayService,
                                       std::shared_ptr<RecordService> recordService)
    : m_gameRouter(std::move(gameRouter)),
      m_gamePlayService(std::move(gamePlayService)),
      m_recordService(std::move(recordService)) {}

void FtxuiGameOverScene::onEnter() {
    m_isRecord = m_recordService->isNewRecord(buildRecord(), 10);
}

void FtxuiGameOverScene::draw() {
    const auto& gamePlay = m_gamePlayService->getGamePlay();

    auto stats = hbox({
        vbox({
            text("Movimentos") | dim | hcenter,
            text(std::to_string(gamePlay.getNumberOfMoves())) | bold | color(Color::Yellow) | hcenter,
        }),
        text("      "),
        vbox({
            text("Tempo") | dim | hcenter,
            text(formatMillis(gamePlay.getDurationMillis())) | bold | color(Color::Yellow) | hcenter,
        }),
    }) | hcenter;

    Elements body = {
        text("Você resolveu o puzzle!") | bold | color(Color::Green) | hcenter,
        text(" "),
        stats,
        text(" "),
    };

    std::string hintText;
    if (m_isRecord) {
        body.push_back(text("★ NOVO RECORDE! ★") | bold | color(Color::Yellow) | hcenter);
        body.push_back(text(" "));
        body.push_back(text("Digite seu nome:") | hcenter);
        body.push_back(
            text(" " + m_name + "▌ ") | color(Color::Black) | bgcolor(Color::Cyan)
            | size(WIDTH, EQUAL, 24) | hcenter);
        hintText = "digite o nome   BACKSPACE apagar   ENTER confirmar";
    } else {
        body.push_back(text("Sem recorde desta vez — tente de novo!") | dim | hcenter);
        hintText = "ENTER voltar ao menu";
    }

    present(vbox({
        filler(),
        window(text(" FIM DE JOGO "), vbox(std::move(body))) | size(WIDTH, EQUAL, 56) | hcenter,
        filler(),
        hints(hintText),
    }));
}

void FtxuiGameOverScene::input() {
    const KeyEvent event = readKey();

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

Record FtxuiGameOverScene::buildRecord() const {
    const auto& gamePlay = m_gamePlayService->getGamePlay();
    return {gamePlay.getNumberOfMoves(), gamePlay.getDurationMillis()};
}

Record FtxuiGameOverScene::buildRecordAndAssignRecord() const {
    const auto& gamePlay = m_gamePlayService->getGamePlay();
    Record record{gamePlay.getNumberOfMoves(), gamePlay.getDurationMillis()};
    record.assignRecord(m_name.empty() ? "Anônimo" : m_name, gamePlay.getStartedAtAsString());
    return record;
}
