#pragma once
#include <memory>

namespace cengine::routing { class ISceneRepository; }

class GameRouter;
class GamePlayService;
class RecordService;

// Registra as cenas The-Forge nos mesmos codigos de estado das plataformas
// terminal e FTXUI — a maquina de estados (StateGameFlow) e o dominio nao
// mudam.
class ForgeSceneFactory {
public:
    static void populateForgeScenes(
        cengine::routing::ISceneRepository &sceneRepository,
        const std::shared_ptr<GameRouter> &gameRouter,
        const std::shared_ptr<GamePlayService> &gamePlayService,
        const std::shared_ptr<RecordService> &recordService);
};
