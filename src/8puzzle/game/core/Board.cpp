#include <algorithm>
#include <random>
#include "Board.h"

Board::Board() {
    createRandomGame();
}

Board::Board(std::vector<Tile>&& fakeBoard) : board(std::move(fakeBoard)) {
}

const std::vector<Tile>& Board::getBoard() const {
    return board;
}

void Board::createRandomGame() {
	board.clear();

	for (int index = 1; index <= 8; index++) {
        auto tile = Tile(index, std::to_string(index));
        board.emplace_back(tile);
    }
    auto emptyTile = Tile();
    board.emplace_back(emptyTile);

    auto rng = std::default_random_engine{std::random_device{}()};

    constexpr int num_shuffles = 200;
    int empty_tile_pos = 8;

    for (int i = 0; i < num_shuffles; ++i) {
        std::vector<int> possible_move_indices;

        if (empty_tile_pos - 3 >= 0) {
            possible_move_indices.push_back(empty_tile_pos - 3);
        }

        if (empty_tile_pos + 3 < 9) {
            possible_move_indices.push_back(empty_tile_pos + 3);
        }

        if (empty_tile_pos % 3 != 0) {
            possible_move_indices.push_back(empty_tile_pos - 1);
        }

        if ((empty_tile_pos + 1) % 3 != 0) {
            possible_move_indices.push_back(empty_tile_pos + 1);
        }

        if (!possible_move_indices.empty()) {
            std::uniform_int_distribution<> distrib(0, possible_move_indices.size() - 1);
            const int chosen_tile_index = possible_move_indices[distrib(rng)];

            std::swap(board[empty_tile_pos], board[chosen_tile_index]);
            empty_tile_pos = chosen_tile_index;
        }
    }
}

std::pair<int, int> Board::getEmptyPosition() const
{
    for (int index = 0; index < board.size(); index++) {
        if (board[index].isEmpty()) {
            return { index / columns, index % columns };
        }
    }
    return { -1, -1 };
}

std::pair<int, int> Board::getTilePosition(const int idTile) const {
    for (int index = 0; index < board.size(); index++) {
        if (board[index].getId() == idTile) {
            return {index / columns, index % columns};
        }
    }
    return {-1, -1};
}

bool Board::moveTile(const int idTile) {
    std::pair<int, int> emptyPosition = getEmptyPosition();
    std::pair<int, int> emptyTile = getTilePosition(idTile);

    const int emptyLine = emptyPosition.first;
    const int emptyColumn = emptyPosition.second;

    const int lineTile = emptyTile.first;
    const int columnTile = emptyTile.second;

    const int dLine = std::abs(emptyLine - lineTile);
    const int dColumn = std::abs(emptyColumn - columnTile);

    if ((dLine == 1 && dColumn == 0) || (dLine == 0 && dColumn == 1)) {
        std::swap(
            board[index(emptyLine, emptyColumn)],
            board[index(lineTile, columnTile)]
        );
        return true;
    }
    return false;
}

bool Board::isSolved() const
{
    std::vector<int> currentState;
    for (const auto& tile : board) {
        currentState.push_back(tile.getId());
    }

    std::vector<std::vector<int>> validSolutions = {
        {0, 1, 2, 3, 4, 5, 6, 7, 8},
        {1, 2, 3, 4, 5, 6, 7, 8, 0},
        {0, 8, 7, 6, 5, 4, 3, 2, 1},
        {8, 7, 6, 5, 4, 3, 2, 1, 0}
    };
    

    return std::any_of(validSolutions.begin(), validSolutions.end(),
                   [&](const auto& solution) {
                       return currentState == solution;
                   });
}

int Board::index(const int line, const int column) const
{
     return line * columns + column;
}


