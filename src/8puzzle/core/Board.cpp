#include <algorithm>
#include <random>
#include "Board.h"

Board::Board() {
    createRandomGame();
}

Board::Board(std::vector<Tile>&& fakeBoard) :
    board(std::move(fakeBoard)) {
}

void Board::createRandomGame() {
	board.clear();

	for (int index = 1; index <= 8; index++) {
        auto tile = Tile(index, std::to_string(index));
        board.emplace_back(tile);
    }
    auto emptyTile = Tile();
    board.emplace_back(emptyTile);

    auto rng = std::default_random_engine{ std::random_device{}() };
    std::shuffle(board.begin(), board.end(), rng);
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

std::pair<int, int> Board::getTilePosition(int idTile) const
{
    for (int index = 0; index < board.size(); index++) {
        if (board[index].getId() == idTile) {
            return { index / columns, index % columns };
        }
    }
    return { -1, -1 };
}


bool Board::moveTile(int idTile) {
    std::pair<int, int> emptyPosition = getEmptyPosition();
    std::pair<int, int> emptyTile = getTilePosition(idTile);

    const int emptyLine = emptyPosition.first;
    const int emptyColumn = emptyPosition.second;

    const int lineTile = emptyTile.first;
    const int columnTile = emptyTile.second;

    const int dLine = std::abs(emptyLine - lineTile);
    const int dColumn = std::abs(emptyColumn - columnTile);

    if ((dLine == 1 && dColumn == 0) || (dLine == 0 && dColumn == 1)) {
        // Troca as pe�as
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

std::string Board::toString() const {
    std::string result;

    // Cabe�alho das colunas
    result += "   0   1   2\n";
    result += " +---+---+---+\n";

    for (int line = 0; line < 3; ++line) {
        result += std::to_string(line) + "|";

        for (int column = 0; column < 3; ++column) {
            int index = line * 3 + column;
            const Tile& tile = board[index];
            std::string value = tile.isEmpty() ? " " : tile.toString();

            // Garante que o texto ocupa 3 caracteres
            result += " " + value + " |";
        }

        result += "\n +---+---+---+\n";
    }

    return result;
}


