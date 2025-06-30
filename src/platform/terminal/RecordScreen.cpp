#include "RecordScreen.h"
#include <conio.h>
#include <iomanip>
#include <iostream>
#include <vector>

#include "8puzzle/domain/GameRouter.h"
#include "8puzzle/service/RecordService.h"
#include "platform/ServiceContainer.h"

void RecordScreen::draw() {
    std::cout << "=== RECORDES ===\n";
    std::cout << "(T)empo | (M)ovimentos | (ESC) Voltar\n\n";

    const auto recordService = ServiceContainer::get().getRecordService();

    std::vector<Record> records;

    if (m_sortType == RecordSortType::ByTime) {
        records = recordService->listByFastestTime();
        std::cout << "Ordenado por: Tempo\n\n";
    } else {
        records = recordService->listByFewestMoves();
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

void RecordScreen::input() {
    if (_kbhit()) {
        char key = _getch();

        switch (key) {
            case 't':
            case 'T':
                m_sortType = RecordSortType::ByTime;
                break;
            case 'm':
            case 'M':
                m_sortType = RecordSortType::ByMoves;
                break;
            case 27: // ESC
                getGameRouter().menu();
                break;
        }
    }
}