#pragma once
#include "8puzzle/engine/IState.h"

class IRouter;
class StateGameFlow : public IState {

public:
    StateGameFlow() = default;
    ~StateGameFlow() override = default;

    [[nodiscard]] std::string getCode() const override = 0;
    [[nodiscard]] std::string getName() const override = 0;
    [[nodiscard]] std::unique_ptr<IState> clone() const override = 0;

    virtual void introduction(IRouter& game) = 0;
    virtual void menu(IRouter& game) = 0;
    virtual void game(IRouter& game) = 0;
    virtual void gameOver(IRouter& game) = 0;
    virtual void record(IRouter& game) = 0;
    virtual void exit(IRouter& game) = 0;

};