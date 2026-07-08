#pragma once

#include <string>

#include "RecordRepository.h"

// Persistência dos recordes em arquivo texto, uma linha por recorde, campos
// separados por TAB: nome, movimentos, duração (ms) e data/hora da partida.
// TAB é seguro como separador: a captura de nome só aceita caracteres
// imprimíveis (isprint), que excluem tab.
class FileRecordRepository final : public RecordRepository {
    std::string m_filePath;

public:
    explicit FileRecordRepository(std::string filePath);

    /// Arquivo inexistente = nenhum recorde ainda; linhas malformadas são
    /// ignoradas (arquivo corrompido não derruba o jogo).
    [[nodiscard]] std::vector<Record> loadAll() override;

    void saveAll(const std::vector<Record>& records) override;
};
