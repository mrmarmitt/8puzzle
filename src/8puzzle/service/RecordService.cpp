#include "RecordService.h"

void RecordService::loadInitialRecords(const std::vector<Record>& initialRecords){
    m_records = initialRecords;
}

void RecordService::addRecord(const Record& record) {
    m_records.push_back(record);
}

std::vector<Record> RecordService::listByFastestTime() const {
    std::vector<Record> sorted = m_records;
    std::sort(sorted.begin(), sorted.end(), [](const Record& a, const Record& b) {
        return a.isFasterThan(b);
    });
    return sorted;
}

std::vector<Record> RecordService::listByFewestMoves() const {
    std::vector<Record> sorted = m_records;
    std::sort(sorted.begin(), sorted.end(), [](const Record& a, const Record& b) {
        return a.hasLessMovesThan(b);
    });
    return sorted;
}

const std::vector<Record>& RecordService::getAll() const {
    return m_records;
}

bool RecordService::isNewRecord(const Record& newRecord, int minPosition) const {
    const auto sortedByFaster = listByFastestTime();
    const auto sortedByMoves= listByFewestMoves();

    if (m_records.size() < minPosition) {
        return true;
    }
    const Record& worstTimeTop = sortedByFaster[minPosition - 1];
    const Record& worstMoveTop = sortedByMoves[minPosition - 1];

    if (newRecord.isFasterThan(worstTimeTop)) {
        return true;
    }

    if (newRecord.hasLessMovesThan(worstMoveTop)) {
        return true;
    }

    return false;
}