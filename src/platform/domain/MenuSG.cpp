#include "MenuSG.h"

#include "ApplicationGame.h"
#include "ExitSG.h"

MenuSG::MenuSG() = default;

void MenuSG::exit(ApplicationGame& game) {
    game.setState(std::make_unique<ExitSG>());
}
