#pragma once
#include <memory>

#include "../engine/service/RouterService.h"

class StateGameFlow;

class GameRouter final : public RouterService {

    static StateGameFlow& castIt(IState& state) ;
public:
    explicit GameRouter(std::shared_ptr<SceneRepository> sceneRepository);
    ~GameRouter() override;

    void introduction();
    void menu();
    void game();
    void gameOver();
    void record();
    void exit();

};

