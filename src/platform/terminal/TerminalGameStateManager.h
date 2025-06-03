#pragma once
#include "../IGameStateManager.h"
#include "../../8puzzle/logic/PuzzleGameManage.h"

class TerminalGameStateManager : public IGameStateManager {
private:
    PuzzleGameManage puzzleGameManage;
    //map(GameState, IGameObject)

public:
    TerminalGameStateManager();

    void input() override;
    void render() override;
    void cleanup() override;
};