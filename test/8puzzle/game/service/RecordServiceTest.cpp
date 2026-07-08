#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "8puzzle/game/Record.h"
#include "8puzzle/game/service/RecordService.h"
#include "8puzzle/game/service/repository/FileRecordRepository.h"
#include "8puzzle/game/service/repository/RecordRepository.h"

namespace {

// Repositório em memória: prova que o service fala só com a porta.
class InMemoryRecordRepository final : public RecordRepository {
public:
    std::vector<Record> stored;
    int saveCount = 0;

    std::vector<Record> loadAll() override { return stored; }

    void saveAll(const std::vector<Record>& records) override {
        stored = records;
        ++saveCount;
    }
};

} // namespace

TEST(RecordServiceTest, LoadsPersistedRecordsOnConstruction) {
    auto repository = std::make_shared<InMemoryRecordRepository>();
    repository->stored = {Record("Ana", 12, "2026-07-08 10:00:00", 45000)};

    const RecordService service(repository);

    ASSERT_EQ(service.getAll().size(), 1);
    EXPECT_EQ(service.getAll()[0].getName(), "Ana");
}

TEST(RecordServiceTest, AddRecordPersistsImmediately) {
    auto repository = std::make_shared<InMemoryRecordRepository>();
    RecordService service(repository);

    service.addRecord(Record("Bia", 20, "2026-07-08 11:00:00", 60000));

    EXPECT_EQ(repository->saveCount, 1);
    ASSERT_EQ(repository->stored.size(), 1);
    EXPECT_EQ(repository->stored[0].getName(), "Bia");
}

// ---------------------------------------------------------------------------
// FileRecordRepository: round-trip em arquivo temporário.
// ---------------------------------------------------------------------------
class FileRecordRepositoryTest : public ::testing::Test {
protected:
    std::string m_path;

    void SetUp() override {
        m_path = (std::filesystem::temp_directory_path() / "8puzzle_records_test.tsv").string();
        std::filesystem::remove(m_path);
    }

    void TearDown() override {
        std::filesystem::remove(m_path);
    }
};

TEST_F(FileRecordRepositoryTest, LoadAllReturnsEmptyWhenFileDoesNotExist) {
    FileRecordRepository repository(m_path);

    EXPECT_TRUE(repository.loadAll().empty());
}

TEST_F(FileRecordRepositoryTest, SaveAllThenLoadAllRoundTrips) {
    FileRecordRepository repository(m_path);

    repository.saveAll({
        Record("Ana", 12, "2026-07-08 10:00:00", 45123),
        Record("Bia com espacos", 20, "2026-07-08 11:30:00", 60000),
        Record("", 7, "2026-07-08 12:00:00", 30500), // nome vazio é válido
    });

    const auto loaded = repository.loadAll();

    ASSERT_EQ(loaded.size(), 3);
    EXPECT_EQ(loaded[0].getName(), "Ana");
    EXPECT_EQ(loaded[0].getNumberOfMoves(), 12);
    EXPECT_EQ(loaded[0].getTime(), 45123);
    EXPECT_EQ(loaded[0].getGamingAt(), "2026-07-08 10:00:00");
    EXPECT_EQ(loaded[1].getName(), "Bia com espacos");
    EXPECT_EQ(loaded[2].getName(), "");
    EXPECT_EQ(loaded[2].getTime(), 30500);
}

TEST_F(FileRecordRepositoryTest, LoadAllSkipsMalformedLines) {
    {
        std::ofstream file(m_path);
        file << "Ana\t12\t45123\t2026-07-08 10:00:00\n";
        file << "linha corrompida sem separadores\n";
        file << "Caio\tnao-e-numero\t100\t2026-07-08 13:00:00\n";
        file << "Duda\t9\t20000\t2026-07-08 14:00:00\n";
    }

    FileRecordRepository repository(m_path);
    const auto loaded = repository.loadAll();

    ASSERT_EQ(loaded.size(), 2);
    EXPECT_EQ(loaded[0].getName(), "Ana");
    EXPECT_EQ(loaded[1].getName(), "Duda");
}
