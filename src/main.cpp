#include <iostream>
#include "8puzzle/core/Tabuleiro.h"
#include <platform/EngineManager.h>
#include <platform/IWindowManager.h>
#include <platform/terminal/WindowManager.h>
#include <platform/terminal/TerminalGameStateManager.h>
#include <platform/terminal/MenuGameObject.h>
#include <platform/IGameStateManager.h>
#include <8puzzle/logic/views/ViewState.h>
#include <8puzzle/logic/PuzzleGameManage.h>
    

using namespace std;

int main()
{
    //std::unique_ptr<IWindowManager> windowManager = std::make_unique<WindowManager>();
    //std::unique_ptr<IGameStateManager> gameStateManager = std::make_unique<TerminalGameStateManager>();
    //std::unique_ptr<FPSManager> fpsManager = std::make_unique<FPSManager>(10);
    // 
    //std::unique_ptr<IGameObject> menuGameObject = std::make_unique<MenuGameObject>();

    ////gameStateManager->addGameObject(GameState::Menu, std::move(menuGameObject));
    ////gameStateManager->addGameObject(GameState::Playing, NULL);
    ////gameStateManager->addGameObject(GameState::GameOver, NULL);

    ////gameStateManager->setCurrentState(GameState::Menu);

    //EngineManager engineManager = EngineManager(
    //    std::move(windowManager), 
    //    NULL,
    //    std::move(fpsManager));

    //engineManager.start();



    PuzzleGameManage puzzleGameManage = PuzzleGameManage();
    
    puzzleGameManage.getCurrentView();
       


	return 0;
}