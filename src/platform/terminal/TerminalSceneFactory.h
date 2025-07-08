#pragma once

class SceneContainer;
class GameRouter;

class TerminalSceneFactory {
public:
    static void populateTerminalScreens(SceneContainer& container, GameRouter& gameRouter);
};