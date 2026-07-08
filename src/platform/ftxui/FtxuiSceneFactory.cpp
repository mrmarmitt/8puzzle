#include "FtxuiSceneFactory.h"

#include <cengine/routing/ISceneRepository.hpp>

#include "8puzzle/game/GameRouter.h"

#include "scene/FtxuiExitScene.h"
#include "scene/FtxuiMenuScene.h"
#include "scene/InstructionsScene.h"
#include "scene/PlaceholderScene.h"
#include "scene/SplashScene.h"

// As factories rodam LAZY (no primeiro getScene de cada estado) — capturas
// por VALOR, como na TerminalSceneFactory.
void FtxuiSceneFactory::populateFtxuiScenes(cengine::routing::ISceneRepository &sceneRepository,
                                            const std::shared_ptr<GameRouter> &gameRouter,
                                            const std::shared_ptr<GamePlayService> &gamePlayService) {
    sceneRepository.registerFactory("initial", [gameRouter]() {
        return std::make_unique<SplashScene>(gameRouter);
    });
    sceneRepository.registerFactory("introduction", [gameRouter]() {
        return std::make_unique<InstructionsScene>(gameRouter);
    });
    sceneRepository.registerFactory("menu", [gameRouter, gamePlayService]() {
        return std::make_unique<FtxuiMenuScene>(gameRouter, gamePlayService);
    });
    sceneRepository.registerFactory("game", [gameRouter]() {
        return std::make_unique<PlaceholderScene>("Novo Jogo", [gameRouter] { gameRouter->menu(); });
    });
    sceneRepository.registerFactory("gameOver", [gameRouter]() {
        return std::make_unique<PlaceholderScene>("Game Over", [gameRouter] { gameRouter->menu(); });
    });
    sceneRepository.registerFactory("record", [gameRouter]() {
        return std::make_unique<PlaceholderScene>("Recordes", [gameRouter] { gameRouter->menu(); });
    });
    sceneRepository.registerFactory("exit", []() {
        return std::make_unique<FtxuiExitScene>();
    });
}
