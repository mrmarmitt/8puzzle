#pragma once
#include <string>

class Tile {
    int id_;
    std::string description_;

public:
    Tile();
    Tile(int id, const std::string& description);

    [[nodiscard]] int getId() const;
    [[nodiscard]] const std::string& getDescription() const;
    [[nodiscard]] bool isEmpty() const;

    [[nodiscard]] std::string toString() const;
};