#pragma once

#include "Peca.h"
#include <vector>
#include <utility>

class Tabuleiro {
private:
    std::vector<Peca> tabuleiro;
    const int linhas = 3;
    const int colunas = 3;

    int index(int linha, int coluna) const;
    void criarJogoRandomico();
public:
    Tabuleiro();
    Tabuleiro(std::vector<Peca> tabuleiroFabricado);

    std::pair<int, int> getPosicaoVazia() const;
    std::pair<int, int> getPosicaoPeca(int idPeca) const;

    bool moverPeca(int idPeca);
    bool estaResolvido() const;

    std::string toString() const;
};