#pragma once

#include <vector>

#include "8puzzle/game/Record.h"

class RecordService {
    std::vector<Record> m_records;

public:
    RecordService() = default;

    void loadInitialRecords(const std::vector<Record>& initialRecords);

    void addRecord(const Record& record);

    [[nodiscard]] std::vector<Record> listByFastestTime() const;
    [[nodiscard]] std::vector<Record> listByFewestMoves() const;
    [[nodiscard]] const std::vector<Record>& getAll() const;
    [[nodiscard]] bool isNewRecord(const Record& newRecord, int minPosition) const;
};
