#pragma once
#include <iostream>

#include "IStateGame.h"

class InitialSG final : public IStateGame {
public:
    InitialSG();

    [[nodiscard]] GameState getGameState() const override { return Initial; }
    [[nodiscard]] std::string getGameStateName() const override { return "Initial"; }
    void introduction(ApplicationGame& game) override;
    void menu(ApplicationGame& game) override { std::cout << "It is not possible to select menu from initial state" << std::endl; }
    void exit(ApplicationGame& game) override { std::cout << "It is not possible to exit from initial state" << std::endl; }
};
