#pragma once
#include <cengine/routing/IState.hpp>

// Mantém o nome `IState` que o jogo usa, agora apontando para o tipo da cengine.
// Desde a cengine 0.2.0 os estados são movidos (unique_ptr) para o router —
// sem clone()/Prototype.
using cengine::routing::IState;

class GameRouter;

class StateGameFlow : public IState {

public:
    StateGameFlow() = default;
    ~StateGameFlow() override = default;

    [[nodiscard]] std::string getCode() const override = 0;
    [[nodiscard]] std::string getName() const override = 0;

    // Transições da máquina de estados (const: estados não têm dados — apenas
    // despacham a próxima navegação no GameRouter).
    virtual void introduction(GameRouter& game) const = 0;
    virtual void menu(GameRouter& game) const = 0;
    virtual void game(GameRouter& game) const = 0;
    virtual void gameOver(GameRouter& game) const = 0;
    virtual void record(GameRouter& game) const = 0;
    virtual void exit(GameRouter& game) const = 0;

};
