#pragma once
#include <iostream>

#include "IStateGame.h"

class MenuSG final : public IStateGame {
public:
    MenuSG();

    [[nodiscard]] GameState getGameState() const override { return Menu; }
    [[nodiscard]] std::string getGameStateName() const override { return "Menu"; }
    void introduction(ApplicationGame& game) override { std::cout << "It is not possible to select introduction from menu state" << std::endl; }
    void menu(ApplicationGame& game) override { std::cout << "It is not possible to select menu from menu state" << std::endl; }
    void exit(ApplicationGame& game) override;
};
