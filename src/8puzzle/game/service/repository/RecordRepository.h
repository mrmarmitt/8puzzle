#pragma once

#include <vector>

#include "8puzzle/game/Record.h"

// Porta de persistência dos recordes: o domínio só conhece carregar/salvar a
// lista completa; onde e como ela vive (arquivo, memória...) é da implementação.
class RecordRepository {
public:
    virtual ~RecordRepository() = default;

    /// Carrega todos os recordes persistidos (vazio se nada foi salvo ainda).
    [[nodiscard]] virtual std::vector<Record> loadAll() = 0;

    /// Persiste a lista completa de recordes (substitui o conteúdo anterior).
    virtual void saveAll(const std::vector<Record>& records) = 0;
};
