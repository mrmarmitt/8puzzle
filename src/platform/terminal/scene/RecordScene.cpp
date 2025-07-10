#include <conio.h>
#include <iomanip>
#include <iostream>
#include <vector>
#include "RecordScene.h"

#include "../../../8puzzle/engine/ServiceContainer.h"
#include "8puzzle/game/GameRouter.h"
#include "8puzzle/game/service/RecordService.h"

RecordScene::RecordScene() :
    m_gameRouter(ServiceContainer::get().getRouter()),
    m_recordService(ServiceContainer::get().getRecordService()) {}

void RecordScene::draw() {
    std::cout << "=== RECORDES ===\n";
    std::cout << "(T)empo | (M)ovimentos | (ESC) Voltar\n\n";

    std::vector<Record> records;

    if (m_sortType == RecordSortType::ByTime) {
        records = m_recordService->listByFastestTime();
        std::cout << "Ordenado por: Tempo\n\n";
    } else {
        records = m_recordService->listByFewestMoves();
        std::cout << "Ordenado por: Movimentos\n\n";
    }

    std::cout << std::left << std::setw(15) << "Jogador"
              << std::setw(12) << "Movimentos"
              << std::setw(10) << "Tempo(ms)"
              << std::setw(20) << "Data/Hora" << "\n";

    std::cout << "---------------------------------------------------------\n";

    for (const auto& record : records) {
        std::cout << std::left << std::setw(15) << record.getName()
                  << std::setw(12) << record.getNumberOfMoves()
                  << std::setw(10) << record.getTime()
                  << std::setw(20) << record.getGamingAt() << "\n";
    }

    std::cout << std::endl;
}

void RecordScene::input() {
    if (_kbhit()) {
        switch (int key = _getch()) {
            case 't':
            case 'T':
                m_sortType = RecordSortType::ByTime;
                break;
            case 'm':
            case 'M':
                m_sortType = RecordSortType::ByMoves;
                break;
            case 27: // ESC
                m_gameRouter->menu();
                break;
            default:;
        }
    }
}