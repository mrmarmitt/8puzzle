#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "8puzzle/game/core/Board.h"
#include "8puzzle/game/core/Tile.h"


// Cria um tabuleiro resolvido com 1-8 e 0 no final
Board criarTabuleiroResolvido() {
    std::vector<Tile> tiles;
    for (int i = 1; i <= 8; ++i) {
        tiles.emplace_back(i, std::to_string(i));
    }
    tiles.emplace_back(); // pe�a vazia (id = 0)
    return Board(std::move(tiles));
}

// Cria tabuleiro com vazio no in�cio
Board criarTabuleiroVazioInicio() {
    std::vector<Tile> tiles;
    tiles.emplace_back(); // vazio
    for (int i = 1; i <= 8; ++i) {
        tiles.emplace_back(i, std::to_string(i));
    }
    return Board(std::move(tiles));
}

// Teste se tabuleiro resolvido � reconhecido
TEST(TabuleiroTest, TabuleiroResolvidoFinal) {
    const Board board = criarTabuleiroResolvido();
    EXPECT_TRUE(board.isSolved());
}

TEST(TabuleiroTest, TabuleiroResolvidoInicio) {
    const Board board = criarTabuleiroVazioInicio();
    EXPECT_TRUE(board.isSolved());
}

// Testa se pe�a � movida corretamente
TEST(TabuleiroTest, MovePecaValida) {
    // tabuleiro: 1 2 3 4 5 6 7 0 8 (vazio em [2][1])
    std::vector<Tile> tiles = {
        Tile(1, "1"), Tile(2, "2"), Tile(3, "3"),
        Tile(4, "4"), Tile(5, "5"), Tile(6, "6"),
        Tile(7, "7"), Tile(0, " "), Tile(8, "8")
    };

    Board board(std::move(tiles));

    EXPECT_TRUE(board.moveTile(8));
    EXPECT_TRUE(board.isSolved());
}

// Testa se movimento inv�lido n�o muda nada
TEST(TabuleiroTest, NaoMovePecaInvalida) {
    Board board = criarTabuleiroResolvido();
    EXPECT_FALSE(board.moveTile(1)); // 1 n�o est� ao lado da pe�a vazia
    EXPECT_TRUE(board.isSolved()); // ainda est� resolvido
}