#include <string>
#include <vector>

#include "8puzzle/game/core/Board.h"
#include "8puzzle/game/core/Tile.h"
#include "gtest/gtest.h"

// Cria um tabuleiro resolvido com 1-8 e 0 no final
Board criarTabuleiroResolvido() {
    std::vector<Tile> pecas;
    for (int i = 1; i <= 8; ++i) {
        pecas.emplace_back(i, std::to_string(i));
    }
    pecas.emplace_back(); // pe�a vazia (id = 0)
    return Board(std::move(pecas));
}

// Cria tabuleiro com vazio no in�cio
Board criarTabuleiroVazioInicio() {
    std::vector<Tile> pecas;
    pecas.emplace_back(); // vazio
    for (int i = 1; i <= 8; ++i) {
        pecas.emplace_back(i, std::to_string(i));
    }
    return Board(std::move(pecas));
}

// Teste se tabuleiro resolvido � reconhecido
TEST(TabuleiroTest, TabuleiroResolvidoFinal) {
    Board tab = criarTabuleiroResolvido();
    EXPECT_TRUE(tab.isSolved());
}

TEST(TabuleiroTest, TabuleiroResolvidoInicio) {
    Board tab = criarTabuleiroVazioInicio();
    EXPECT_TRUE(tab.isSolved());
}

// Testa se pe�a � movida corretamente
TEST(TabuleiroTest, MovePecaValida) {
    // tabuleiro: 1 2 3 4 5 6 7 0 8 (vazio em [2][1])
    std::vector<Tile> pecas = {
        Tile(1, "1"), Tile(2, "2"), Tile(3, "3"),
        Tile(4, "4"), Tile(5, "5"), Tile(6, "6"),
        Tile(7, "7"), Tile(0, " "), Tile(8, "8")
    };

    Board tab(std::move(pecas));

    EXPECT_TRUE(tab.moveTile(8));
    EXPECT_TRUE(tab.isSolved());
}

// Testa se movimento inv�lido n�o muda nada
TEST(TabuleiroTest, NaoMovePecaInvalida) {
    Board tab = criarTabuleiroResolvido();
    EXPECT_FALSE(tab.moveTile(1)); // 1 n�o est� ao lado da pe�a vazia
    EXPECT_TRUE(tab.isSolved()); // ainda est� resolvido
}