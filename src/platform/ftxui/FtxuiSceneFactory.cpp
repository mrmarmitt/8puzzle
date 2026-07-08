#include "FtxuiSceneFactory.h"

#include <cengine/routing/ISceneRepository.hpp>

#include "8puzzle/game/GameRouter.h"

#include "scene/FtxuiExitScene.h"
#include "scene/FtxuiGameOverScene.h"
#include "scene/FtxuiGameScene.h"
#include "scene/FtxuiMenuScene.h"
#include "scene/FtxuiRecordScene.h"
#include "scene/InstructionsScene.h"
#include "scene/SplashScene.h"

// As factories rodam LAZY (no primeiro getScene de cada estado) — capturas
// por VALOR, como na TerminalSceneFactory.
void FtxuiSceneFactory::populateFtxuiScenes(cengine::routing::ISceneRepository &sceneRepository,
                                            const std::shared_ptr<GameRouter> &gameRouter,
                                            const std::shared_ptr<GamePlayService> &gamePlayService,
                                            const std::shared_ptr<RecordService> &recordService) {
    sceneRepository.registerFactory("initial", [gameRouter]() {
        return std::make_unique<SplashScene>(gameRouter);
    });
    sceneRepository.registerFactory("introduction", [gameRouter]() {
        return std::make_unique<InstructionsScene>(gameRouter);
    });
    sceneRepository.registerFactory("menu", [gameRouter, gamePlayService]() {
        return std::make_unique<FtxuiMenuScene>(gameRouter, gamePlayService);
    });
    sceneRepository.registerFactory("game", [gameRouter, gamePlayService]() {
        return std::make_unique<FtxuiGameScene>(gameRouter, gamePlayService);
    });
    sceneRepository.registerFactory("gameOver", [gameRouter, gamePlayService, recordService]() {
        return std::make_unique<FtxuiGameOverScene>(gameRouter, gamePlayService, recordService);
    });
    sceneRepository.registerFactory("record", [gameRouter, recordService]() {
        return std::make_unique<FtxuiRecordScene>(gameRouter, recordService);
    });
    sceneRepository.registerFactory("exit", []() {
        return std::make_unique<FtxuiExitScene>();
    });
}
