#pragma once
#include <iostream>

#include "IStateGame.h"

class IntroductionSG final : public IStateGame {
public:
    IntroductionSG() = default;

    [[nodiscard]] GameState getGameState() const override { return Introduction; }
    [[nodiscard]] std::string getGameStateName() const override { return "Introduction"; }
    void introduction(ApplicationGame& game) override { std::cout << "It is not possible to select introduction from introduction state" << std::endl; }
    void menu(ApplicationGame& game) override;
    void exit(ApplicationGame& game) override { std::cout << "It is not possible to exit from introduction state" << std::endl; }
};

