#include "TerminalScreenFactory.h"

#include "../../8puzzle/engine/ScreenContainer.h"
#include "8puzzle/game/state/StateEnum.h"
#include "screen/ExitScreen.h"
#include "screen/GameOverScreen.h"
#include "screen/GameScreen.h"
#include "screen/InitialScreen.h"
#include "screen/IntroductionScreen.h"
#include "screen/MenuScreen.h"
#include "screen/RecordScreen.h"

void TerminalScreenFactory::populateTerminalScreens(ScreenContainer& container, GameRouter& gameRouter) {
    static InitialScreen initialScreen(gameRouter);
    static IntroductionScreen introductionScreen(gameRouter);
    static MenuScreen menuScreen(gameRouter);
    static GameScreen gameScreen(gameRouter);
    static GameOverScreen gameOverScreen(gameRouter);
    static RecordScreen recordScreen(gameRouter);
    static ExitScreen exitScreen(gameRouter);

    container.registerScreen("initial", &initialScreen);
    container.registerScreen("introduction", &introductionScreen);
    container.registerScreen("menu", &menuScreen);
    container.registerScreen("game", &gameScreen);
    container.registerScreen("gameOver", &gameOverScreen);
    container.registerScreen("record", &recordScreen);
    container.registerScreen("exit", &exitScreen);

}
