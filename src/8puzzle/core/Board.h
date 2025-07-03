#pragma once

#include <utility>
#include <vector>
#include "Tile.h"

class Board {
    std::vector<Tile> board;
    const int lines = 3;
    const int columns = 3;

    [[nodiscard]] int index(int line, int column) const;
public:
    Board();
    explicit Board(std::vector<Tile>&& fakeBoard);

    void createRandomGame();
    [[nodiscard]] const std::vector<Tile>& getBoard() const;

    [[nodiscard]] std::pair<int, int> getEmptyPosition() const;
    [[nodiscard]] std::pair<int, int> getTilePosition(int idTile) const;

    bool moveTile(int idTile);
    [[nodiscard]] bool isSolved() const;


    // [[nodiscard]] std::string toString() const;
};