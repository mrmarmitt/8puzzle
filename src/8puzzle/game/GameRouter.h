#pragma once
#include <memory>

#include <cengine/routing/IState.hpp>
#include <cengine/routing/IRouter.hpp>

class StateGameFlow;

// Fachada de navegação de domínio do 8Puzzle sobre o roteador da cengine.
// As cenas chamam introduction()/menu()/game()/... que delegam à máquina de
// estados (StateGameFlow do estado atual) e agendam a próxima cena via
// setNextState(). Desde a cengine 0.2.0 o repositório de cenas pertence ao
// router; a fachada fala apenas com o IRouter.
class GameRouter final {
    std::shared_ptr<cengine::routing::IRouter> m_router;

    static const StateGameFlow& castIt(const cengine::routing::IState& state);

public:
    explicit GameRouter(std::shared_ptr<cengine::routing::IRouter> router);
    ~GameRouter();

    // Agenda o próximo estado/cena (chamado pelas transições de StateGameFlow).
    void setNextState(std::unique_ptr<cengine::routing::IState> state) const;

    void introduction();
    void menu();
    void game();
    void gameOver();
    void record();
    void exit();
};
