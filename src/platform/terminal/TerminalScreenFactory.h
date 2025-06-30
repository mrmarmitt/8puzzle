#pragma once

class ScreenContainer;
class GameRouter;

class TerminalScreenFactory {
public:
    static void populateTerminalScreens(ScreenContainer& container, GameRouter& gameRouter);
};