#pragma once
#include <cengine/routing/IState.hpp>

// Mantém o nome `IState` que o jogo usa, agora apontando para o tipo da cengine
// (estado do jogo, com clone() = padrão Prototype).
using cengine::routing::IState;

class GameRouter;

class StateGameFlow : public IState {

public:
    StateGameFlow() = default;
    ~StateGameFlow() override = default;

    [[nodiscard]] std::string getCode() const override = 0;
    [[nodiscard]] std::string getName() const override = 0;
    [[nodiscard]] std::unique_ptr<IState> clone() const override = 0;

    virtual void introduction(GameRouter& game) = 0;
    virtual void menu(GameRouter& game) = 0;
    virtual void game(GameRouter& game) = 0;
    virtual void gameOver(GameRouter& game) = 0;
    virtual void record(GameRouter& game) = 0;
    virtual void exit(GameRouter& game) = 0;

};
