#include "Peca.h"

Peca::Peca() : id_(0), descricao_(" ") {}

Peca::Peca(int id, const std::string& descricao)
    : id_(id), descricao_(descricao) {
}

int Peca::getId() const {
    return id_;
}

const std::string& Peca::getDescricao() const {
    return descricao_;
}

bool Peca::estaVazio() const {
    return id_ == 0;
}

std::string Peca::toString() const {
    return std::to_string(id_);
}