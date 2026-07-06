#pragma once
#include <memory>

#include <cengine/routing/IState.hpp>
#include <cengine/routing/ISceneRepository.hpp>

class StateGameFlow;

// Fachada de navegação de domínio do 8Puzzle sobre o repositório de cenas da
// cengine. As cenas chamam introduction()/menu()/game()/... que delegam à
// máquina de estados (StateGameFlow do próximo estado) e agendam a próxima cena
// via setNextState(). O loop em si é conduzido por um cengine::routing::IRouter
// que compartilha o mesmo ISceneRepository (ver main.cpp).
class GameRouter final {
    std::shared_ptr<cengine::routing::ISceneRepository> m_sceneRepository;

    static StateGameFlow& castIt(cengine::routing::IState& state);

public:
    explicit GameRouter(std::shared_ptr<cengine::routing::ISceneRepository> sceneRepository);
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
