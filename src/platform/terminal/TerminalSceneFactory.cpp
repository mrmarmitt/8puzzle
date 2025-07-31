#include "TerminalSceneFactory.h"

#include "../../8puzzle/engine/service/repository/SceneRepository.h"
#include "8puzzle/engine/service/RouterService.h"
#include "scene/ExitScene.h"
#include "scene/GameOverScene.h"
#include "scene/GameScene.h"
#include "scene/InitialScene.h"
#include "scene/IntroductionScene.h"
#include "scene/MenuScene.h"
#include "scene/RecordScene.h"

void TerminalSceneFactory::populateTerminalScreens(const std::shared_ptr<SceneRepository> &sceneRepository,
                                                   const std::shared_ptr<GameRouter> &gameRouter,
                                                   const std::shared_ptr<ConfigurationService> &configurationService,
                                                   const std::shared_ptr<GamePlayService> &gamePlayService,
                                                   const std::shared_ptr<RecordService> &recordService) {
    sceneRepository->registerFactory("initial", [&]() { return std::make_unique<InitialScene>(gameRouter); });
    sceneRepository->registerFactory("introduction", [&]() { return std::make_unique<IntroductionScene>(gameRouter); });
    sceneRepository->registerFactory("menu", [&]() { return std::make_unique<MenuScene>(gameRouter, gamePlayService); });
    sceneRepository->registerFactory("game", [&]() { return std::make_unique<GameScene>(gameRouter, gamePlayService); });
    sceneRepository->registerFactory("gameOver", [&]() { return std::make_unique<GameOverScene>(gameRouter, gamePlayService, recordService); });
    sceneRepository->registerFactory("record", [&]() { return std::make_unique<RecordScene>(gameRouter, recordService); });
    sceneRepository->registerFactory("exit", [&]() { return std::make_unique<ExitScene>(); });
}
