#include "MenuGameObject.h"

#include <iostream>

using namespace std;

MenuGameObject::MenuGameObject(): m_menu(Menu()) {
}

MenuGameObject::~MenuGameObject() {
    cleanup();
}

void MenuGameObject::input() {
    if (_kbhit()) {
        char key = _getch();
        switch (key) {
            case 72:
                m_menu.moveUp();
                break; // Seta ↑
            case 80:
                m_menu.moveDown();
                break; // Seta ↓
            case 13:
                //m_menu.selectOption();               
                break; // Enter
        }
    }
}

void MenuGameObject::render() {
    gotoxy(0, 10); // Posição do menu

    // Título
    std::cout << "=== MENU PRINCIPAL ===" << std::endl << std::endl;

    // Opções (obtidas do objeto Menu)
    const auto& options = m_menu.getOptions();
    int selected = m_menu.getIndexSelectedOption();

    for (size_t i = 0; i < options.size(); i++) {
        if (i == selected) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 112); // Destaque
            std::cout << "> " << options[i] << std::endl;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // Reset
        } else {
            std::cout << "  " << options[i] << std::endl;
        }
    }
}

void MenuGameObject::cleanup() {
    cout << "Cleanup menu game object." << endl;
}

void MenuGameObject::gotoxy(int x, int y) {
    COORD coord = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
