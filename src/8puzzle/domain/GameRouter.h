#pragma once
#include <memory>

#include "8puzzle/service/RecordService.h"
#include "state/StateEnum.h"

class TerminalScreenCache;
class GamePlay;
class IScreen;
class ScreenCacheManager;
class IStateGame;

class GameRouter final {
    std::unique_ptr<IStateGame> m_currentState;

public:
    GameRouter();
    ~GameRouter();

    void setState(std::unique_ptr<IStateGame> state);

    [[nodiscard]] IStateGame& getCurrentStateGame() const;
    [[nodiscard]] std::string getCurrentStateGameName() const;
    [[nodiscard]] StateEnum getCurrentStateGameEnum() const;
    [[nodiscard]] IScreen& getCurrentCachedScreen() const;

    void introduction();
    void menu();
    void game();
    void gameOver();
    void record();
    void exit();
};

