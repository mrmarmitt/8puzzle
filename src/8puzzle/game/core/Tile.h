#pragma once
#include <string>

class Tile {
    int id_;
    std::string description_;

public:
    Tile();
    Tile(int id, const std::string& description);

    int getId() const;
    const std::string& getDescription() const;
    bool isEmpty() const;

    std::string toString() const;
};