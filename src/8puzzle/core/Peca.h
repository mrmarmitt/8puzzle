#pragma once
#include <string>

class Peca {
private:
    int id_;
    std::string descricao_;

public:
    Peca();
    Peca(int id, const std::string& descricao);

    int getId() const;
    const std::string& getDescricao() const;
    bool estaVazio() const;

    std::string toString() const;
};