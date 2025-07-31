#include "MenuScene.h"

#include <conio.h>
#include <windows.h>
#include <iostream>
#include <vector>

#include "8puzzle/game/GameRouter.h"
#include "8puzzle/game/service/GamePlayService.h"

MenuScene::MenuScene(std::shared_ptr<GameRouter> gameRouter,
                     std::shared_ptr<GamePlayService> gamePlayService) : m_gameRouter(std::move(gameRouter)),
                                                                         m_gamePlayService(std::move(gamePlayService)) {
}

void MenuScene::draw() {
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

void MenuScene::input() {
    if (_kbhit()) {
        const int key = _getch();
        constexpr int numOptions = 4;

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
                        m_gamePlayService->startNewGame();
                        m_gameRouter->game();
                        break;
                    case 1: // Recorde
                        m_gameRouter->record();
                        break;
                    case 2: // Configuracoes
                        // getGameRouter().configuration();
                        break;
                    case 3: // Sair
                        m_gameRouter->exit(); // Chama o método exit na ApplicationGame
                        break;
                    default: ;
                }
                break;
            case 27:
                m_gameRouter->exit(); // Chama o método exit na ApplicationGame
                break;
            default:
                break;
        }
    }
}
