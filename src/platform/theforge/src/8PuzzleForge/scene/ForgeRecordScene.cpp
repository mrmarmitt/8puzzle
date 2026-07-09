#include "ForgeRecordScene.h"

#include <string>
#include <utility>
#include <vector>

#include "8puzzle/game/GameRouter.h"
#include "8puzzle/game/Record.h"
#include "8puzzle/game/service/RecordService.h"
#include "platform/Format.h"

namespace {

// Colunas da tabela: deslocamento a partir da margem esquerda do bloco.
constexpr float kColPosition = 0.0f;
constexpr float kColPlayer = 60.0f;
constexpr float kColMoves = 340.0f;
constexpr float kColTime = 500.0f;
constexpr float kColDate = 660.0f;
constexpr float kTableWidth = 880.0f;
constexpr float kRowHeight = 30.0f;
constexpr float kHeaderSize = 20.0f;
constexpr float kRowSize = 19.0f;

} // namespace

ForgeRecordScene::ForgeRecordScene(std::shared_ptr<GameRouter> gameRouter,
                                   std::shared_ptr<RecordService> recordService)
    : m_gameRouter(std::move(gameRouter)),
      m_recordService(std::move(recordService)) {}

void ForgeRecordScene::draw() {
    const bool byTime = (m_sortType == ForgeRecordSort::ByTime);
    const std::vector<Record> records =
        byTime ? m_recordService->listByFastestTime()
               : m_recordService->listByFewestMoves();

    const float h = forgeui::screenHeight();
    const float left = (forgeui::screenWidth() - kTableWidth) * 0.5f;

    forgeui::drawTextCentered("RECORDES", h * 0.10f, 44.0f, forgeui::color::kTitle);
    forgeui::drawTextCentered(byTime ? "Ordenado por: Tempo" : "Ordenado por: Movimentos",
                              h * 0.10f + 60.0f, 20.0f, forgeui::color::kDim);

    float y = h * 0.28f;
    forgeui::drawText("#", left + kColPosition, y, kHeaderSize, forgeui::color::kText);
    forgeui::drawText("Jogador", left + kColPlayer, y, kHeaderSize, forgeui::color::kText);
    forgeui::drawText("Movimentos", left + kColMoves, y, kHeaderSize,
                      byTime ? forgeui::color::kText : forgeui::color::kAccent);
    forgeui::drawText("Tempo", left + kColTime, y, kHeaderSize,
                      byTime ? forgeui::color::kAccent : forgeui::color::kText);
    forgeui::drawText("Data/Hora", left + kColDate, y, kHeaderSize, forgeui::color::kText);
    y += kRowHeight + 6.0f;

    if (records.empty()) {
        forgeui::drawTextCentered("Nenhum recorde ainda - jogue uma partida!", y + 20.0f, 22.0f,
                                  forgeui::color::kDim);
    } else {
        int position = 1;
        for (const auto& record : records) {
            forgeui::drawText(std::to_string(position++), left + kColPosition, y, kRowSize, forgeui::color::kDim);
            forgeui::drawText(record.getName(), left + kColPlayer, y, kRowSize, forgeui::color::kText);
            forgeui::drawText(std::to_string(record.getNumberOfMoves()), left + kColMoves, y, kRowSize,
                              forgeui::color::kValue);
            forgeui::drawText(formatMillis(record.getTime()), left + kColTime, y, kRowSize, forgeui::color::kValue);
            forgeui::drawText(record.getGamingAt(), left + kColDate, y, kRowSize, forgeui::color::kDim);
            y += kRowHeight;
        }
    }

    forgeui::drawHints("T ordenar por tempo   M ordenar por movimentos   ESC menu");
}

void ForgeRecordScene::input() {
    const KeyEvent event = forgeui::readKey();

    switch (event.key) {
        case Key::Char:
            if (event.character == 't' || event.character == 'T') {
                m_sortType = ForgeRecordSort::ByTime;
            } else if (event.character == 'm' || event.character == 'M') {
                m_sortType = ForgeRecordSort::ByMoves;
            }
            break;
        case Key::Escape:
            m_gameRouter->menu();
            break;
        default:
            break;
    }
}
