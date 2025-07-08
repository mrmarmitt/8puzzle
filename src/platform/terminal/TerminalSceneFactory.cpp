#include "TerminalSceneFactory.h"

#include "../../8puzzle/engine/SceneContainer.h"
#include "8puzzle/game/state/StateEnum.h"
#include "scene/ExitScene.h"
#include "scene/GameOverScene.h"
#include "scene/GameScene.h"
#include "scene/InitialScene.h"
#include "scene/IntroductionScene.h"
#include "scene/MenuScene.h"
#include "scene/RecordScene.h"

void TerminalSceneFactory::populateTerminalScreens(SceneContainer& container, GameRouter& gameRouter) {
    static InitialScene initialScreen(gameRouter);
    static IntroductionScene introductionScreen(gameRouter);
    static MenuScene menuScreen(gameRouter);
    static GameScene gameScreen(gameRouter);
    static GameOverScene gameOverScreen(gameRouter);
    static RecordScene recordScreen(gameRouter);
    static ExitScene exitScreen(gameRouter);

    container.registerScene("initial", &initialScreen);
    container.registerScene("introduction", &introductionScreen);
    container.registerScene("menu", &menuScreen);
    container.registerScene("game", &gameScreen);
    container.registerScene("gameOver", &gameOverScreen);
    container.registerScene("record", &recordScreen);
    container.registerScene("exit", &exitScreen);

}
