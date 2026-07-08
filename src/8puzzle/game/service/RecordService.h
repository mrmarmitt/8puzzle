#pragma once

#include <memory>
#include <vector>

#include "8puzzle/game/Record.h"

class RecordRepository;

class RecordService {
    std::shared_ptr<RecordRepository> m_recordRepository;
    std::vector<Record> m_records; // cache em memória do conteúdo persistido

public:
    /// Carrega os recordes persistidos na construção.
    explicit RecordService(std::shared_ptr<RecordRepository> recordRepository);

    /// Adiciona e persiste imediatamente (recorde não pode se perder ao fechar).
    void addRecord(const Record& record);

    [[nodiscard]] std::vector<Record> listByFastestTime() const;
    [[nodiscard]] std::vector<Record> listByFewestMoves() const;
    [[nodiscard]] const std::vector<Record>& getAll() const;
    [[nodiscard]] bool isNewRecord(const Record& newRecord, int minPosition) const;
};
