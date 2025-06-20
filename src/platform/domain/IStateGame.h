#pragma once
#include "GameState.cpp"

class ApplicationGame;

class IStateGame {
public:
    virtual ~IStateGame() = default;

    [[nodiscard]] virtual GameState getGameState() const = 0;
    [[nodiscard]] virtual std::string getGameStateName() const = 0;
    virtual void introduction(ApplicationGame& game) = 0;
    virtual void menu(ApplicationGame& game) = 0;
    virtual void exit(ApplicationGame& game) = 0;
};
