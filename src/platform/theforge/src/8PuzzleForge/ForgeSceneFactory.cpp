#include "ForgeSceneFactory.h"

#include <cengine/routing/ISceneRepository.hpp>

#include "8puzzle/game/GameRouter.h"

#include "scene/ForgeExitScene.h"
#include "scene/ForgeGameOverScene.h"
#include "scene/ForgeGameScene.h"
#include "scene/ForgeInstructionsScene.h"
#include "scene/ForgeMenuScene.h"
#include "scene/ForgeRecordScene.h"
#include "scene/ForgeSplashScene.h"

// As factories rodam LAZY (no primeiro getScene de cada estado) — capturas
// por VALOR, como nas demais plataformas.
void ForgeSceneFactory::populateForgeScenes(cengine::routing::ISceneRepository &sceneRepository,
                                            const std::shared_ptr<GameRouter> &gameRouter,
                                            const std::shared_ptr<GamePlayService> &gamePlayService,
                                            const std::shared_ptr<RecordService> &recordService) {
    sceneRepository.registerFactory("initial", [gameRouter]() {
        return std::make_unique<ForgeSplashScene>(gameRouter);
    });
    sceneRepository.registerFactory("introduction", [gameRouter]() {
        return std::make_unique<ForgeInstructionsScene>(gameRouter);
    });
    sceneRepository.registerFactory("menu", [gameRouter, gamePlayService]() {
        return std::make_unique<ForgeMenuScene>(gameRouter, gamePlayService);
    });
    sceneRepository.registerFactory("game", [gameRouter, gamePlayService]() {
        return std::make_unique<ForgeGameScene>(gameRouter, gamePlayService);
    });
    sceneRepository.registerFactory("gameOver", [gameRouter, gamePlayService, recordService]() {
        return std::make_unique<ForgeGameOverScene>(gameRouter, gamePlayService, recordService);
    });
    sceneRepository.registerFactory("record", [gameRouter, recordService]() {
        return std::make_unique<ForgeRecordScene>(gameRouter, recordService);
    });
    sceneRepository.registerFactory("exit", []() {
        return std::make_unique<ForgeExitScene>();
    });
}
