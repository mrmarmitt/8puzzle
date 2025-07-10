#pragma once
#include <memory>

#include "8puzzle/engine/IRouter.h"

class StateGameFlow;

class GameRouter final : public IRouter {

    static StateGameFlow& castIt(const std::unique_ptr<IState>& state) ;
public:
    GameRouter();
    ~GameRouter() override;

    void introduction();
    void menu();
    void game();
    void gameOver();
    void record();
    void exit();

};

