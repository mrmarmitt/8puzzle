#pragma once
#include <iostream>

#include "IStateGame.h"

class ExitSG final : public IStateGame {
public:
    ExitSG();

    [[nodiscard]] GameState getGameState() const override { return Exit; }
    [[nodiscard]] std::string getGameStateName() const override { return "Exit"; }
    void introduction(ApplicationGame& game) override { std::cout << "It is not possible to select introduction from exit state" << std::endl; }
    void menu(ApplicationGame& game) override { std::cout << "It is not possible to select menu from exit state" << std::endl; }
    void exit(ApplicationGame& game) override {
        std::cout << "It is not possible to select exit from exit state" << std::endl;
    }

};
