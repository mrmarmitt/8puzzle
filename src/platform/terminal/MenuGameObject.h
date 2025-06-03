#pragma once

#include <vector>
#include <utility>
#include <windows.h>
#include <conio.h>

#include "../../8puzzle/screen/Menu.h"

#include "../IGameObject.h"

class MenuGameObject : public IGameObject {
private:
    Menu m_menu;

    void gotoxy(int x, int y);
    //GameState getGameState(int index);


public:
    MenuGameObject();
    ~MenuGameObject();

    void input() override;
    void render() override;
    void cleanup() override;
};

