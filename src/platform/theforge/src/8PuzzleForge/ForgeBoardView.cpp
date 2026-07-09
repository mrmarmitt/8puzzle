#include "ForgeBoardView.h"

#include "8puzzle/game/core/Board.h"
#include "8puzzle/game/core/Tile.h"
#include "ForgeUi.h"

void drawTileGrid(const std::array<std::string, 9>& labels, const float centerX, const float topY, const float cellSize)
{
    const float left = centerX - cellSize * 1.5f;
    const float fontSize = cellSize * 0.55f;

    for (int line = 0; line < 3; ++line)
    {
        for (int column = 0; column < 3; ++column)
        {
            const std::string& label = labels[line * 3 + column];
            const bool         empty = label.empty();
            const std::string  shown = empty ? "." : label;
            const uint32_t     colorAbgr = empty ? forgeui::color::kFaint : forgeui::color::kTitle;

            // centraliza o rotulo dentro da celula
            const float cellX = left + (float)column * cellSize;
            const float cellY = topY + (float)line * cellSize;
            const float x = cellX + (cellSize - forgeui::textWidth(shown, fontSize)) * 0.5f;
            const float y = cellY + cellSize * 0.18f;
            forgeui::drawText(shown, x, y, fontSize, colorAbgr);
        }
    }
}

void drawBoard(const Board& board, const float centerX, const float topY, const float cellSize)
{
    std::array<std::string, 9> labels;
    for (int i = 0; i < 9; ++i)
    {
        const Tile& tile = board.getBoard()[i];
        labels[i] = tile.isEmpty() ? "" : tile.toString();
    }
    drawTileGrid(labels, centerX, topY, cellSize);
}
