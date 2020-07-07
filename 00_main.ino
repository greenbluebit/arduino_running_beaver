#include "setup.h"
#include "elements.h"
#include "inputs.h"
#include "game.h"

void setup() {
  game::SetupGame();
}

void loop() {
  game::LoopGame();
}
