#pragma once
#include <memory>

#include "8puzzle/service/RecordService.h"
#include "platform/IGameManager.h"
#include "platform/ScreenCache.h"

class TerminalGameManager final : public IGameManager {
    std::unique_ptr<ApplicationGame> m_applicationGame;
    std::unique_ptr<ScreenCache> m_screenCache;
    std::unique_ptr<RecordService> m_recordService;

public:
    TerminalGameManager();
    ~TerminalGameManager() override = default;

    void input() override;
    void render() override;
    auto shouldExist() -> bool override;
    void cleanup() override;
};