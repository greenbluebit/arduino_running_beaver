#pragma once


namespace game {
  unsigned long current_time = 0;

  elements::Player* player = new elements::Player(elements::Player_State::standing);

                                            // PIN, PRESS, HOLD, RELEASE
  inputs::Button* btnOne = new inputs::Button(BTN_PIN_ONE,
  [&player]() { // PRESS
    player->Jump();
    if(currentGameState == Game_State::stopped || currentGameState == Game_State::over) {
      currentGameState = Game_State::running;
    }
  }, 
  [&player]() { // HOLD
  }, 
  [&player, &currentGameState]() { // RELEASE
    player->Fall(); 
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
    topObstacleOne,
    bottomObstacleTwo,
    topObstacleTwo,
    bottomObstacleThree,
    bottomObstacleFour,
    topObstacleThree,
    topObstacleFour
  };
  int obstacleIndex = 0;
  unsigned long lastTimeObstacleSpawn = 0;
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

    
    if(currentGameState == Game_State::running && current_time - last_obstacle_movement_time > (OBSTACLE_MOVEMENT_INTERVAL * obstacle_movement_multiplier)) {// ALL INPUT HERE
      last_obstacle_movement_time = current_time;
      (*bottomObstacleOne).Move();
      (*bottomObstacleTwo).Move();
      (*bottomObstacleThree).Move();
      (*bottomObstacleFour).Move();

      (*topObstacleOne).Move();
      (*topObstacleTwo).Move();
      (*topObstacleThree).Move();
      (*topObstacleFour).Move();
    }
    
  }

  void RenderLogic() {
    // HANDLE BUTTON CHANGE STATES
    (*btnOne).OnRender();
    switch(currentGameState) {
      case Game_State::stopped :
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.write("RUNNING BEAVER");
        lcd.setCursor(5, 1);
        lcd.write("start!");
        
      break;
      case Game_State::running :
        lcd.clear();
        // PUT ALL RENDER CALLS HERE        
        (*player).Render();
    
        // obstacle spawn logic
        if(gameStarted == false) {
          gameStarted = true;
          (*player).rendering = true;
          score = 0;
          obstacle_movement_multiplier = 1;
          last_multiplier_change = current_time;
          // if(current_time - lastTimeObstacleSpawn > NEW_GAME_OBSTACLE_SPAWN_INTERVAL) {
          //   if((*obstacles[obstacleIndex]).rendering == false) {
          //     (*obstacles[obstacleIndex]).Reset();
          //   }
            
          //   obstacleIndex = obstacleIndex >= 8 ? 0 : obstacleIndex + 1;
          //   lastTimeObstacleSpawn = current_time;
          //   gameStarted = true;
          //   (*player).rendering = true;
          // }
        }
        if(gameStarted == true && current_time - lastTimeObstacleSpawn > (OBSTACLE_SPAWN_INTERVAL * obstacle_movement_multiplier)) {
          if(random(0, 100) < OBSTACLE_SPAWN_CHANCE + chanceModifier) {
            
            if((*obstacles[obstacleIndex]).rendering == false) {
              (*obstacles[obstacleIndex]).Reset();
              lastTimeObstacleSpawn = current_time;
            }
            obstacleIndex = obstacleIndex >= 8 ? 0 : obstacleIndex + 1;
            chanceModifier = 0;
          } else {
            chanceModifier = chanceModifier < MAX_CHANCE_MODIFIER ? chanceModifier + 20 : MAX_CHANCE_MODIFIER;
            obstacleIndex = obstacleIndex >= 8 ? 0 : obstacleIndex + 1;
          }
          // lastTimeObstacleSpawn = current_time;
        }
        if(current_time - last_multiplier_change > MULTIPLIER_CHANGE_INTERVAL && obstacle_movement_multiplier > OBSTACLE_MAX_MOVEMENT_MULTIPLER) {
          obstacle_movement_multiplier = obstacle_movement_multiplier - OBSTACLE_MOVEMENT_MULTIPLIER_CHANGER;
          last_multiplier_change = current_time;
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
        
        last_score_time = 0;
        lastTimeObstacleSpawn = 0;
        chanceModifier = 0;
        obstacleIndex = 0;
        
        (*bottomObstacleOne).rendering = false;
        (*bottomObstacleTwo).rendering = false;
        (*bottomObstacleThree).rendering = false;
        (*bottomObstacleFour).rendering = false;

        (*topObstacleOne).rendering = false;
        (*topObstacleTwo).rendering = false;
        (*topObstacleThree).rendering = false;
        (*topObstacleFour).rendering = false;
        
        lcd.clear();
        // setCursor(x, y)
        lcd.setCursor(0,0);
        lcd.write("Score: ");
        lcd.setCursor(7, 0);
        lcd.write(String(score).c_str());
        lcd.setCursor(3,1);
        lcd.write("Try again?");
      break;
    }
  }
}
