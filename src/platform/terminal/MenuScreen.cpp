#include "MenuScreen.h"

#include <conio.h>
#include <iostream>
#include <vector>

#include "TerminalWindowManager.h"

void MenuScreen::draw(ApplicationGame& applicationGame) {
    std::cout << "=== MENU PRINCIPAL ===" << std::endl << std::endl;

    const std::vector<std::string> options = {"Novo Jogo", "Recorde", "Configuracoes", "Sair"};

    for (size_t i = 0; i < options.size(); i++) {
        if (i == m_selected) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 112); // Destaque
            std::cout << "> " << options[i] << std::endl;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // Reset
        } else {
            std::cout << "  " << options[i] << std::endl;
        }
    }
}

void MenuScreen::input(ApplicationGame& applicationGame) {
    if (_kbhit()) {
        char key = _getch();

        const int numOptions = 4;

        switch (key) {
            case 72: // Seta para CIMA
                m_selected = (m_selected == 0) ? (numOptions - 1) : (m_selected - 1);
                break;
            case 80: // Seta para BAIXO
                m_selected = (m_selected == numOptions - 1) ? 0 : (m_selected + 1);
                break;
            case 13: // ENTER
                switch (m_selected) {
                    case 0:
                        applicationGame.game();
                        break;
                    case 1: // Recorde
                        applicationGame.record();
                        break;
                    case 2: // Configuracoes
                        // applicationGame.configuration();
                        break;
                    case 3: // Sair
                        applicationGame.exit(); // Chama o método exit na ApplicationGame
                        break;
                }
                break;
            default:
                break;
        }
    }
}