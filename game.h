#pragma once


namespace game {
  unsigned long current_time = 0;

  elements::Player* player = new elements::Player(elements::Player_State::standing);

                                            // PIN, PRESS, HOLD, RELEASE
  inputs::Button* btnOne = new inputs::Button(BTN_PIN_ONE,
  [&player]() { // PRESS
    player->Jump(); 
  }, 
  [&player]() { // HOLD
    //Serial.println("HOLD"); 
  }, 
  [&player, &currentGameState]() { // RELEASE
    player->Fall(); 
    if(currentGameState == Game_State::stopped || currentGameState == Game_State::over) {
      currentGameState = Game_State::running;
    }
  }
  );

  //                                                 Obstacle(int startPosX, int startPosY, int startSize, Player* startPlayer, bool startCanMove, bool startMovesLeft, int startObstacleType){
  elements::Obstacle* bottomObstacleOne = new elements::Obstacle(16,1,1, player,true, true, 0);
  elements::Obstacle* bottomObstacleTwo = new elements::Obstacle(16,1,1, player,true, true, 0);
  elements::Obstacle* bottomObstacleThree = new elements::Obstacle(16,1,1, player,true, true, 0);
  elements::Obstacle* bottomObstacleFour = new elements::Obstacle(16,1,1, player,true, true, 0);

  elements::Obstacle* topObstacleOne = new elements::Obstacle(16,0,1, player,true, true, 1);
  elements::Obstacle* topObstacleTwo = new elements::Obstacle(16,0,1, player,true, true, 1);
  elements::Obstacle* topObstacleThree = new elements::Obstacle(16,0,1, player,true, true, 1);
  elements::Obstacle* topObstacleFour = new elements::Obstacle(16,0,1, player,true, true, 1);

  elements::Obstacle* obstacles[] = {
    bottomObstacleOne,
    bottomObstacleTwo,
    bottomObstacleThree,
    bottomObstacleFour,
    topObstacleOne,
    topObstacleTwo,
    topObstacleThree,
    topObstacleFour
  };
  int obstacleIndex = 0;
  unsigned long lastTimeObstacleSpawn = 0;
  unsigned long OBSTACLE_SPAWN_INTERVAL = 600;
  unsigned long NEW_GAME_OBSTACLE_SPAWN_INTERVAL = 1000;
  int OBSTACLE_SPAWN_CHANCE = 50; // from 0 to 100
  int chanceModifier = 0;
  int MAX_CHANCE_MODIFIER = 500;
  
  bool gameStarted = false;
  // PROTOTYPES
  void RenderLogic();
  
  void SetupGame() {
    Serial.begin(9600);
    lcd.begin(16, 2);
    
    // TODO Try to skip index 0 and start from 1, might be faster, no idea why
    lcd.createChar(tailIndex, &sprite[0]);
    lcd.createChar(runOneIndex, &sprite[8]);
    lcd.createChar(runTwoIndex, &sprite[16]);
    lcd.createChar(obstacleTopIndex, obstacleTop);
    lcd.createChar(obstacleBottomIndex, obstacleBottom);
  }

  void StartGame() {
    Serial.println("StartGame");
  }

  void HandleInput() {
    
  }
  
//Score: 999999999    16 chars
//Try again?          10 chars
  void LoopGame() {
    current_time = millis();

    if(current_time - last_logic_time > LOGIC_INTERVAL) {// ALL INPUT HERE
      last_logic_time = current_time;
      (*btnOne).Check();
    }
    if(current_time - last_render_time > RENDER_INTERVAL) {// ALL RENDER
      last_render_time = current_time;
      RenderLogic();
    }
    
  }

  void RenderLogic() {
    // HANDLE BUTTON CHANGE STATES
    (*btnOne).OnRender();
    switch(currentGameState) {
      case Game_State::stopped :
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.write("Press button to ");
        lcd.setCursor(1, 6);
        lcd.write("start!");
      break;
      case Game_State::running :
        lcd.clear();
        // PUT ALL RENDER CALLS HERE        
        (*player).Render();
    
        // obstacle spawn logic
        if(gameStarted == false) {
          if(current_time - lastTimeObstacleSpawn > NEW_GAME_OBSTACLE_SPAWN_INTERVAL) {
            (*obstacles[obstacleIndex]).Reset();
            obstacleIndex = obstacleIndex >= 16 ? 0 : obstacleIndex + 1;
            lastTimeObstacleSpawn = current_time;
          }
        }
        if(current_time - lastTimeObstacleSpawn > OBSTACLE_SPAWN_INTERVAL) {
          if(random(0, 100) < OBSTACLE_SPAWN_CHANCE + chanceModifier) {
            (*obstacles[obstacleIndex]).Reset();
            obstacleIndex = obstacleIndex >= 16 ? 0 : obstacleIndex + 1;
            chanceModifier = 0;
          } else {
            chanceModifier = chanceModifier < MAX_CHANCE_MODIFIER ? chanceModifier + 1 : MAX_CHANCE_MODIFIER;
          }
          lastTimeObstacleSpawn = current_time;
        }
        
        // obstacle render logic
        (*bottomObstacleOne).Render();
        (*bottomObstacleTwo).Render();
        (*bottomObstacleThree).Render();
        (*bottomObstacleFour).Render();

        (*topObstacleOne).Render();
        (*topObstacleTwo).Render();
        (*topObstacleThree).Render();
        (*topObstacleFour).Render();

        if(current_time - last_score_time > SCORING_INTERVAL) {
          last_score_time = current_time;
          score += 1;
          if(score > MAX_SCORE) {
            score = MAX_SCORE;
          }
        }
      break;
      case Game_State::over :
        gameStarted = false;
        score = 0;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.write("Score: " + score);
        lcd.setCursor(1,3);
        lcd.write("Try again?");
      break;
    }
  }
}
