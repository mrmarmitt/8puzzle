#pragma once

#include <vector>
#include <string>
#include "enums/MenuGameState.h"


class Menu {
private:
    std::vector<MenuGameState> m_options;
    int m_selectedIndex = 0;
    MenuGameState m_confirmedOption = MenuGameState::None;

public:
    Menu();

    MenuGameState getSelectedOption() const;
    int getIndexSelectedOption() const;
    MenuGameState getConfirmedOption() const;
    std::vector<std::string> getOptions() const;

    // Lógica do menu
    void moveUp();
    void moveDown();
    void confirm();
};