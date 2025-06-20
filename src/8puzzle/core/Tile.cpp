#include "Tile.h"

Tile::Tile() : id_(0), description_(" ") {}

Tile::Tile(int id, const std::string& description)
    : id_(id), description_(description) {
}

int Tile::getId() const {
    return id_;
}

const std::string& Tile::getDescription() const {
    return description_;
}

bool Tile::isEmpty() const {
    return id_ == 0;
}

std::string Tile::toString() const {
    return std::to_string(id_);
}