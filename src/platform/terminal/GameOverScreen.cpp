#include "GameOverScreen.h"

#include <conio.h>

#include <iostream>

#include "8puzzle/domain/GameRouter.h"

void GameOverScreen::onEnter() {
    // Record& record = gameRouter.getRecordFromEndGame();
    // if (m_recordService->isNewRecord(record, 10)) {
        // m_isRecord = m_recordService->isNewRecord(record, 10);
    // }
}

void GameOverScreen::draw() {
    if (m_isRecord) {
        std::cout << "Parabens! Você fez um novo recorde!\n";
        std::cout << "Digite seu nome para registrar: " << m_name << std::flush;
    } else {
        std::cout << "Voce nao bateu o recorde. Pressione Enter para voltar ao menu.\n";
    }
}
void GameOverScreen::input() {
    if (!_kbhit()) return;

    const char key = _getch();

    if (m_isRecord) {

        if (key == 13) { // ENTER
            // gameRouter.getRecordFromEndGame().assignRecord(m_name, "2025-06-22 18:10");
            // m_recordService->addRecord(gameRouter.getRecordFromEndGame());
            getGameRouter().menu();
        }
        // BACKSPACE remove caractere
        else if (key == 8 && !m_name.empty()) {
            m_name.pop_back();
        }
        // Caracteres visíveis
        else if (std::isprint(static_cast<unsigned char>(key))) {
            if (m_name.size() < 20) { // limite opcional
                m_name.push_back(key);
            }
        }
    } else {
        // Apenas espera Enter
        if (key == 13) { // ENTER
            getGameRouter().menu();
        }
    }
}

void GameOverScreen::onExit() {

}
