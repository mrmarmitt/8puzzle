#pragma once
#include <memory>

namespace cengine::routing { class ISceneRepository; }

class GameRouter;
class GamePlayService;

// Registra as cenas FTXUI nos mesmos códigos de estado da plataforma terminal
// — a máquina de estados (StateGameFlow) e o domínio não mudam.
class FtxuiSceneFactory {
public:
    static void populateFtxuiScenes(
        cengine::routing::ISceneRepository &sceneRepository,
        const std::shared_ptr<GameRouter> &gameRouter,
        const std::shared_ptr<GamePlayService> &gamePlayService);
};
