#include "Tabuleiro.h"
#include <algorithm>
#include <random>

Tabuleiro::Tabuleiro() {
    criarJogoRandomico();
}

Tabuleiro::Tabuleiro(std::vector<Peca> tabuleiroFabricado) {
    tabuleiro = tabuleiroFabricado;
}

void Tabuleiro::criarJogoRandomico() {
	tabuleiro.clear();

	for (int index = 1; index <= 8; index++) {
		Peca peca = Peca(index, std::to_string(index));
		tabuleiro.emplace_back(peca);
	}
    Peca pecaVazia = Peca();
    tabuleiro.emplace_back(pecaVazia);

    auto rng = std::default_random_engine{ std::random_device{}() };
    std::shuffle(tabuleiro.begin(), tabuleiro.end(), rng);
}


std::pair<int, int> Tabuleiro::getPosicaoVazia() const
{
    for (int index = 0; index < tabuleiro.size(); index++) {
        if (tabuleiro[index].estaVazio()) {
            return { index / colunas, index % colunas };
        }
    }
    return { -1, -1 };
}

std::pair<int, int> Tabuleiro::getPosicaoPeca(int idPeca) const
{
    for (int index = 0; index < tabuleiro.size(); index++) {
        if (tabuleiro[index].getId() == idPeca) {
            return { index / colunas, index % colunas };
        }
    }
    return { -1, -1 };
}


bool Tabuleiro::moverPeca(int idPeca) {
    std::pair<int, int> posicaoVazia = getPosicaoVazia();
    std::pair<int, int> posicaoPeca= getPosicaoPeca(idPeca);

    int linhaVazia = posicaoVazia.first;
    int colunaVazia = posicaoVazia.second;

    int linhaPeca = posicaoPeca.first;
    int colunaPeca = posicaoPeca.second;

    int dLinha = std::abs(linhaVazia - linhaPeca);
    int dColuna = std::abs(colunaVazia - colunaPeca);

    if ((dLinha == 1 && dColuna == 0) || (dLinha == 0 && dColuna == 1)) {
        // Troca as peças
        std::swap(
            tabuleiro[index(linhaVazia, colunaVazia)],
            tabuleiro[index(linhaPeca, colunaPeca)]
        );

        return true;
    }

    return false;
}

bool Tabuleiro::estaResolvido() const
{
    std::vector<int> estadoAtual;
    for (const auto& peca : tabuleiro) {
        estadoAtual.push_back(peca.getId());
    }

    std::vector<std::vector<int>> solucoesValidas = {
        {0, 1, 2, 3, 4, 5, 6, 7, 8},
        {1, 2, 3, 4, 5, 6, 7, 8, 0},
        {0, 8, 7, 6, 5, 4, 3, 2, 1},
        {8, 7, 6, 5, 4, 3, 2, 1, 0}
    };
    

    for (const auto& solucao : solucoesValidas) {
        if (estadoAtual == solucao) {
            return true;
        }
    }

    return false;
}

int Tabuleiro::index(int linha, int coluna) const
{
     return linha * colunas + coluna;
}

std::string Tabuleiro::toString() const {
    std::string resultado;

    // Cabeçalho das colunas
    resultado += "   0   1   2\n";
    resultado += " +---+---+---+\n";

    for (int linha = 0; linha < 3; ++linha) {
        resultado += std::to_string(linha) + "|";

        for (int coluna = 0; coluna < 3; ++coluna) {
            int index = linha * 3 + coluna;
            const Peca& peca = tabuleiro[index];
            std::string valor = peca.estaVazio() ? " " : peca.toString();

            // Garante que o texto ocupa 3 caracteres
            resultado += " " + valor + " |";
        }

        resultado += "\n +---+---+---+\n";
    }

    return resultado;
}


