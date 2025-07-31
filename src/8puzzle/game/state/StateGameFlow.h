#pragma once
#include "8puzzle/engine/IState.h"

class RouterService;
class StateGameFlow : public IState {

public:
    StateGameFlow() = default;
    ~StateGameFlow() override = default;

    [[nodiscard]] std::string getCode() const override = 0;
    [[nodiscard]] std::string getName() const override = 0;
    [[nodiscard]] std::unique_ptr<IState> clone() const override = 0;

    virtual void introduction(RouterService& game) = 0;
    virtual void menu(RouterService& game) = 0;
    virtual void game(RouterService& game) = 0;
    virtual void gameOver(RouterService& game) = 0;
    virtual void record(RouterService& game) = 0;
    virtual void exit(RouterService& game) = 0;

};