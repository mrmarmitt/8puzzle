#include "IntroductionSG.h"

#include "ApplicationGame.h"
#include "MenuSG.h"

void IntroductionSG::menu(ApplicationGame& game) {
    game.setState(std::make_unique<MenuSG>());
}


