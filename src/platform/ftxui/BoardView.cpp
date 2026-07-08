#include "BoardView.h"

#include <vector>

#include "8puzzle/game/core/Board.h"
#include "8puzzle/game/core/Tile.h"

using namespace ftxui;

Element tileCell(const std::string& label) {
    const bool empty = label.empty();
    Element cell = text(label) | bold | center
        | size(WIDTH, EQUAL, 7) | size(HEIGHT, EQUAL, 3);
    if (!empty) {
        cell = cell | color(Color::Cyan);
    }
    return cell | borderRounded;
}

Element boardElement(const Board& board) {
    std::vector<Elements> rows;
    for (int line = 0; line < 3; ++line) {
        Elements row;
        for (int column = 0; column < 3; ++column) {
            const Tile& tile = board.getBoard()[line * 3 + column];
            row.push_back(tileCell(tile.isEmpty() ? "" : tile.toString()));
        }
        rows.push_back(std::move(row));
    }
    return gridbox(std::move(rows));
}
