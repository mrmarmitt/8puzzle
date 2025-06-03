#include "gtest/gtest.h"
#include "8puzzle/core/Tabuleiro.h"
#include "8puzzle/core/Peca.h"

// Cria um tabuleiro resolvido com 1-8 e 0 no final
Tabuleiro criarTabuleiroResolvido() {
    std::vector<Peca> pecas;
    for (int i = 1; i <= 8; ++i) {
        pecas.emplace_back(i, std::to_string(i));
    }
    pecas.emplace_back(); // pe�a vazia (id = 0)
    return Tabuleiro(pecas);
}

// Cria tabuleiro com vazio no in�cio
Tabuleiro criarTabuleiroVazioInicio() {
    std::vector<Peca> pecas;
    pecas.emplace_back(); // vazio
    for (int i = 1; i <= 8; ++i) {
        pecas.emplace_back(i, std::to_string(i));
    }
    return Tabuleiro(pecas);
}

// Teste se tabuleiro resolvido � reconhecido
TEST(TabuleiroTest, TabuleiroResolvidoFinal) {
    Tabuleiro tab = criarTabuleiroResolvido();
    EXPECT_TRUE(tab.estaResolvido());
}

TEST(TabuleiroTest, TabuleiroResolvidoInicio) {
    Tabuleiro tab = criarTabuleiroVazioInicio();
    EXPECT_TRUE(tab.estaResolvido());
}

// Testa se pe�a � movida corretamente
TEST(TabuleiroTest, MovePecaValida) {
    // tabuleiro: 1 2 3 4 5 6 7 0 8 (vazio em [2][1])
    std::vector<Peca> pecas = {
        Peca(1, "1"), Peca(2, "2"), Peca(3, "3"),
        Peca(4, "4"), Peca(5, "5"), Peca(6, "6"),
        Peca(7, "7"), Peca(0, " "), Peca(8, "8")
    };
    Tabuleiro tab(pecas);
    EXPECT_TRUE(tab.moverPeca(8));
    EXPECT_TRUE(tab.estaResolvido());
}

// Testa se movimento inv�lido n�o muda nada
TEST(TabuleiroTest, NaoMovePecaInvalida) {
    Tabuleiro tab = criarTabuleiroResolvido();
    EXPECT_FALSE(tab.moverPeca(1)); // 1 n�o est� ao lado da pe�a vazia
    EXPECT_TRUE(tab.estaResolvido()); // ainda est� resolvido
}