#pragma once

#include <utility>
#include <vector>
#include "Tile.h"

class Board {
    std::vector<Tile> board;
    const int lines = 3;
    const int columns = 3;

    int index(int line, int column) const;
    void createRandomGame();
public:
    Board();
    explicit Board(std::vector<Tile>&& fakeBoard);

    std::pair<int, int> getEmptyPosition() const;
    std::pair<int, int> getTilePosition(int idTile) const;

    bool moveTile(int idTile);
    bool isSolved() const;

    std::string toString() const;
};