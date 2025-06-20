#include "InitialSG.h"

#include "ApplicationGame.h"
#include "IntroductionSG.h"

InitialSG::InitialSG() = default;

void InitialSG::introduction(ApplicationGame& game) {
    game.setState(std::make_unique<IntroductionSG>());
}






