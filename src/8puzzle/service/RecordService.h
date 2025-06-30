#pragma once
#include <algorithm>
#include <vector>

#include "8puzzle/domain/Record.h"

class RecordService {
    std::vector<Record> m_records;

public:
    RecordService() = default;

    void loadInitialRecords(const std::vector<Record>& initialRecords);
    // {
    //     m_records = initialRecords;
    // }

    void addRecord(const Record& record);
    // {
    //     m_records.push_back(record);
    // }

    [[nodiscard]] std::vector<Record> listByFastestTime() const;
    // {
    //     std::vector<Record> sorted = m_records;
    //     std::sort(sorted.begin(), sorted.end(), [](const Record& a, const Record& b) {
    //         return a.isFasterThan(b);
    //     });
    //     return sorted;
    // }

    [[nodiscard]] std::vector<Record> listByFewestMoves() const;
    // {
    //     std::vector<Record> sorted = m_records;
    //     std::sort(sorted.begin(), sorted.end(), [](const Record& a, const Record& b) {
    //         return a.hasLessMovesThan(b);
    //     });
    //     return sorted;
    // }

    [[nodiscard]] const std::vector<Record>& getAll() const;
    // {
    //     return m_records;
    // }

    [[nodiscard]] bool isNewRecord(const Record& newRecord, int minPosition) const;
    // {
    //     const auto sortedByFaster = listByFastestTime();
    //     const auto sortedByMoves= listByFewestMoves();
    //
    //     if (m_records.size() < minPosition) {
    //         return true;
    //     }
    //     const Record& worstTimeTop = sortedByFaster[minPosition - 1];
    //     const Record& worstMoveTop = sortedByMoves[minPosition - 1];
    //
    //     if (newRecord.isFasterThan(worstTimeTop)) {
    //         return true;
    //     }
    //
    //     if (newRecord.hasLessMovesThan(worstMoveTop)) {
    //         return true;
    //     }
    //
    //     return false;
    // }
};
