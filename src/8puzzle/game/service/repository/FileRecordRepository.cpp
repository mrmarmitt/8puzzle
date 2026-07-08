#include "FileRecordRepository.h"

#include <fstream>
#include <sstream>
#include <utility>

namespace {

constexpr char kSeparator = '\t';

// Uma linha TSV -> Record; false se a linha não tem os 4 campos válidos.
bool parseLine(const std::string& line, Record& out) {
    std::istringstream in(line);
    std::string name;
    std::string moves;
    std::string millis;
    std::string gamingAt;

    if (!std::getline(in, name, kSeparator)) return false;
    if (!std::getline(in, moves, kSeparator)) return false;
    if (!std::getline(in, millis, kSeparator)) return false;
    if (!std::getline(in, gamingAt)) return false;

    try {
        out = Record(name, std::stoi(moves), gamingAt, std::stoll(millis));
    } catch (...) {
        return false; // números inválidos -> linha ignorada
    }
    return true;
}

} // namespace

FileRecordRepository::FileRecordRepository(std::string filePath)
    : m_filePath(std::move(filePath)) {}

std::vector<Record> FileRecordRepository::loadAll() {
    std::vector<Record> records;

    std::ifstream file(m_filePath);
    if (!file.is_open()) {
        return records; // primeiro uso: nada persistido ainda
    }

    std::string line;
    while (std::getline(file, line)) {
        if (Record record; parseLine(line, record)) {
            records.push_back(record);
        }
    }
    return records;
}

void FileRecordRepository::saveAll(const std::vector<Record>& records) {
    std::ofstream file(m_filePath, std::ios::trunc);
    if (!file.is_open()) {
        return; // sem onde salvar (ex.: diretório sem escrita) — jogo segue
    }

    for (const auto& record : records) {
        file << record.getName() << kSeparator
             << record.getNumberOfMoves() << kSeparator
             << record.getTime() << kSeparator
             << record.getGamingAt() << '\n';
    }
}
