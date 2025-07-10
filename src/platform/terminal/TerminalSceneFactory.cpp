#include "TerminalSceneFactory.h"

#include "../../8puzzle/engine/SceneContainer.h"
#include "scene/ExitScene.h"
#include "scene/GameOverScene.h"
#include "scene/GameScene.h"
#include "scene/InitialScene.h"
#include "scene/IntroductionScene.h"
#include "scene/MenuScene.h"
#include "scene/RecordScene.h"

void TerminalSceneFactory::populateTerminalScreens(SceneContainer& container) {
    container.registerFactory("initial", [&]() { return std::make_unique<InitialScene>(); });
    container.registerFactory("introduction", [&]() { return std::make_unique<IntroductionScene>(); });
    container.registerFactory("menu", [&]() { return std::make_unique<MenuScene>(); });
    container.registerFactory("game", [&]() { return std::make_unique<GameScene>(); });
    container.registerFactory("gameOver", [&]() { return std::make_unique<GameOverScene>(); });
    container.registerFactory("record", [&]() { return std::make_unique<RecordScene>(); });
    container.registerFactory("exit", [&]() { return std::make_unique<ExitScene>(); });
}
