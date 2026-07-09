#include "FtxuiRecordScene.h"

#include <string>
#include <utility>
#include <vector>

#include "8puzzle/game/GameRouter.h"
#include "8puzzle/game/Record.h"
#include "8puzzle/game/service/RecordService.h"
#include "../../Format.h"
#include "../Keyboard.h"

using namespace ftxui;

namespace {

Element cell(const std::string& value, const int width) {
    return text(value) | size(WIDTH, EQUAL, width);
}

Element headerCell(const std::string& value, const int width, const bool active) {
    Element el = text(value) | bold | size(WIDTH, EQUAL, width);
    return active ? el | color(Color::Cyan) : el;
}

} // namespace

FtxuiRecordScene::FtxuiRecordScene(std::shared_ptr<GameRouter> gameRouter,
                                   std::shared_ptr<RecordService> recordService)
    : m_gameRouter(std::move(gameRouter)),
      m_recordService(std::move(recordService)) {}

void FtxuiRecordScene::draw() {
    const bool byTime = (m_sortType == FtxuiRecordSort::ByTime);
    const std::vector<Record> records =
        byTime ? m_recordService->listByFastestTime()
               : m_recordService->listByFewestMoves();

    Elements rows;
    rows.push_back(hbox({
        headerCell("#", 4, false),
        headerCell("Jogador", 22, false),
        headerCell("Movimentos", 12, !byTime),
        headerCell("Tempo", 14, byTime),
        headerCell("Data/Hora", 20, false),
    }));
    rows.push_back(separator());

    if (records.empty()) {
        rows.push_back(text("Nenhum recorde ainda â€” jogue uma partida!") | dim | hcenter);
    } else {
        int position = 1;
        for (const auto& record : records) {
            rows.push_back(hbox({
                cell(std::to_string(position++), 4),
                cell(record.getName(), 22),
                cell(std::to_string(record.getNumberOfMoves()), 12),
                cell(formatMillis(record.getTime()), 14),
                cell(record.getGamingAt(), 20),
            }));
        }
    }

    auto content = vbox({
        text(byTime ? "Ordenado por: Tempo" : "Ordenado por: Movimentos") | dim,
        text(" "),
        vbox(std::move(rows)),
        text(" "),
    });

    present(vbox({
        filler(),
        window(text(" RECORDES "), content) | size(WIDTH, EQUAL, 76) | hcenter,
        filler(),
        hints("T ordenar por tempo   M ordenar por movimentos   ESC menu"),
    }));
}

void FtxuiRecordScene::input() {
    const KeyEvent event = readKey();

    switch (event.key) {
        case Key::Char:
            if (event.character == 't' || event.character == 'T') {
                m_sortType = FtxuiRecordSort::ByTime;
            } else if (event.character == 'm' || event.character == 'M') {
                m_sortType = FtxuiRecordSort::ByMoves;
            }
            break;
        case Key::Escape:
            m_gameRouter->menu();
            break;
        default:
            break;
    }
}
