#include "TerminalSceneFactory.h"

#include <cengine/routing/ISceneRepository.hpp>

#include "scene/ExitScene.h"
#include "scene/GameOverScene.h"
#include "scene/GameScene.h"
#include "scene/InitialScene.h"
#include "scene/IntroductionScene.h"
#include "scene/MenuScene.h"
#include "scene/RecordScene.h"

// As factories rodam LAZY (no primeiro getScene de cada estado), muito depois
// desta função retornar — por isso as lambdas capturam os shared_ptr por VALOR.
void TerminalSceneFactory::populateTerminalScreens(cengine::routing::ISceneRepository &sceneRepository,
                                                   const std::shared_ptr<GameRouter> &gameRouter,
                                                   const std::shared_ptr<ConfigurationService> &configurationService,
                                                   const std::shared_ptr<GamePlayService> &gamePlayService,
                                                   const std::shared_ptr<RecordService> &recordService) {
    (void)configurationService; // ainda sem cena consumidora
    sceneRepository.registerFactory("initial", [gameRouter]() { return std::make_unique<InitialScene>(gameRouter); });
    sceneRepository.registerFactory("introduction", [gameRouter]() { return std::make_unique<IntroductionScene>(gameRouter); });
    sceneRepository.registerFactory("menu", [gameRouter, gamePlayService]() { return std::make_unique<MenuScene>(gameRouter, gamePlayService); });
    sceneRepository.registerFactory("game", [gameRouter, gamePlayService]() { return std::make_unique<GameScene>(gameRouter, gamePlayService); });
    sceneRepository.registerFactory("gameOver", [gameRouter, gamePlayService, recordService]() { return std::make_unique<GameOverScene>(gameRouter, gamePlayService, recordService); });
    sceneRepository.registerFactory("record", [gameRouter, recordService]() { return std::make_unique<RecordScene>(gameRouter, recordService); });
    sceneRepository.registerFactory("exit", []() { return std::make_unique<ExitScene>(); });
}
