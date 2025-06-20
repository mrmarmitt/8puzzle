#pragma once
#include <memory>

#include "platform/IGameManager.h"
#include "platform/ScreenCache.h"

class TerminalGameManager final : public IGameManager {
    std::unique_ptr<ApplicationGame> m_applicationGame;
    std::unique_ptr<ScreenCache> m_screenCache;


public:
    TerminalGameManager();
    ~TerminalGameManager() override = default;

    void input() override;
    void render() override;
    auto shouldExist() -> bool override;
    void cleanup() override;
};