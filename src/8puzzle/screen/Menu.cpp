#include "Menu.h"

Menu::Menu() : m_options({
        MenuGameState::NewGame, 
        MenuGameState::LoadGame, 
        MenuGameState::Records, 
        MenuGameState::Settings, 
        MenuGameState::Exit
      })
{
    m_selectedIndex = 0;
}

MenuGameState Menu::getSelectedOption() const {
    return m_options[m_selectedIndex];
}

int Menu::getIndexSelectedOption() const {
    return m_selectedIndex;
}

MenuGameState Menu::getConfirmedOption() const {
    return m_confirmedOption;
}

std::vector<std::string> Menu::getOptions() const {
    //MenuGameState
    return std::vector<std::string>();
}

void Menu::moveUp() {
    m_selectedIndex = (m_selectedIndex - 1 + m_options.size()) % m_options.size();
}

void Menu::moveDown() {
    m_selectedIndex = (m_selectedIndex + 1) % m_options.size();
}

void Menu::confirm() {
    m_confirmedOption = m_options[m_selectedIndex];
}
