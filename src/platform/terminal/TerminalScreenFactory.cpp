#include "TerminalScreenFactory.h"

#include "8puzzle/domain/state/StateEnum.h"
#include "ExitScreen.h"
#include "GameOverScreen.h"
#include "GameScreen.h"
#include "InitialScreen.h"
#include "IntroductionScreen.h"
#include "MenuScreen.h"
#include "RecordScreen.h"
#include "platform/ScreenContainer.h"

void TerminalScreenFactory::populateTerminalScreens(ScreenContainer& container, GameRouter& gameRouter) {
    static InitialScreen initialScreen(gameRouter);
    static IntroductionScreen introductionScreen(gameRouter);
    static MenuScreen menuScreen(gameRouter);
    static GameScreen gameScreen(gameRouter);
    static GameOverScreen gameOverScreen(gameRouter);
    static RecordScreen recordScreen(gameRouter);
    static ExitScreen exitScreen(gameRouter);

    container.registerScreen(StateEnum::Initial, &initialScreen);
    container.registerScreen(StateEnum::Introduction, &introductionScreen);
    container.registerScreen(StateEnum::Menu, &menuScreen);
    container.registerScreen(StateEnum::Game, &gameScreen);
    container.registerScreen(StateEnum::GameOver, &gameOverScreen);
    container.registerScreen(StateEnum::Record, &recordScreen);
    container.registerScreen(StateEnum::Exit, &exitScreen);

}
