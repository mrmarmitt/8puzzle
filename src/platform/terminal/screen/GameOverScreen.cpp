#include "GameOverScreen.h"

#include <conio.h>

#include <iostream>

#include "../../../8puzzle/engine/ServiceContainer.h"
#include "8puzzle/game/GameRouter.h"

GameOverScreen::GameOverScreen(GameRouter& gameRouter) :
    IScreen(gameRouter),
    m_gamePlayService(ServiceContainer::get().getGameService()),
    m_recordService(ServiceContainer::get().getRecordService()),
    m_isRecord(false){}

void GameOverScreen::onEnter() {
    auto record = buildRecord();
    if (m_recordService->isNewRecord(record, 10)) {
        m_isRecord = m_recordService->isNewRecord(record, 10);
    }
}

void GameOverScreen::draw() {
    if (m_isRecord) {
        std::cout << "Parabens! Voce fez um novo recorde!\n";
        std::cout << "Digite seu nome para registrar: " << m_name << std::flush;
    } else {
        std::cout << "Voce nao bateu o recorde. Pressione Enter para voltar ao menu.\n";
    }
}
void GameOverScreen::input() {
    if (!_kbhit()) return;

    const int key = _getch();

    if (m_isRecord) {

        if (key == 13) { // ENTER

            const auto record = buildRecordAndAssignRecord();
            m_recordService->addRecord(record);

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
    m_isRecord = false;
    m_name = {};
}

Record GameOverScreen::buildRecord() const {
    const auto gamePlay = m_gamePlayService->getCurrentGamePlay();
    auto record = Record(gamePlay->getNumberOfMoves(), gamePlay->getDurationMillis());
    return record;
}

Record GameOverScreen::buildRecordAndAssignRecord() const {
    const auto gamePlay = m_gamePlayService->getCurrentGamePlay();
    auto record = Record(gamePlay->getNumberOfMoves(), gamePlay->getDurationMillis());
    record.assignRecord(m_name, gamePlay->getStartedAtAsString());
    return record;
}