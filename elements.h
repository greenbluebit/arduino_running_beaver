#pragma once


namespace elements {

  struct Element {
    int posX, posY;
    int size;
    bool rendering = false;

    Element(int startPosX, int startPosY, int startSize) {
        posX = startPosX;
        posY = startPosY;
        size = startSize;
    }

    Element() {
        Element(0,0,1);
    }

    void Move(int x, int y) {
        posX = x;
        posY = y;
    }

    void Render();
  };
  
  enum class Player_State
  {
      standing,
      running,
      dead
  };

  struct Player : Element {
      Player_State playerState;
      bool runAnim = false; // On false, it uses first sprite, on true second
      
      Player(Player_State startPlayerState) : Element(0, 1, 2){
          playerState = startPlayerState;
          
      }
  
      void Render() {
        if(rendering == true) {
          lcd.setCursor(posX, posY);
          lcd.write((byte)tailIndex);
          lcd.setCursor(posX + 1, posY);
          lcd.write((byte)(runAnim == true ? runOneIndex : runTwoIndex));
          runAnim = !runAnim;
        }
      }
  
      void Jump() {
        Serial.println("JUMP");
        posY = 0;
      }

      void Fall() {
        posY = 1;
      }
  
      void Die() {
        Serial.println("DIE");
      }
  };

  struct Obstacle : Element {
        //byte sprite[8];
        bool canMove = false;
        bool movesLeft = true; // Don't support moving right yet, due to collision detection
        Player* currentPlayer;
        int obstacleType = 0; // 0 == bottom ; 1 == top

        Obstacle(int startPosX, int startPosY, int startSize, Player* startPlayer) : Element(startPosX, startPosY, startSize) {
          //int spriteLength = sizeof(startSprite) / sizeof(startSprite[0]);
          //for(int i = 0; i < spriteLength; i++) {
          //  sprite[i] = startSprite[i];
          //}
          currentPlayer = startPlayer;
        }

        Obstacle(int startPosX, int startPosY, int startSize, Player* startPlayer, bool startCanMove, bool startMovesLeft, int startObstacleType){
          Obstacle(startPosX, startPosY, startSize, startPlayer);
          canMove = startCanMove;
          movesLeft = startMovesLeft;
          obstacleType = startObstacleType;
        }

        void Render() {
          if(rendering == true ) {
            lcd.setCursor(posX, posY);
            lcd.write((byte) (obstacleType == 0 ? obstacleBottomIndex : obstacleTopIndex));
          }
        }

        void Reset() {
          posX = movesLeft == true ? 16 : 0;
          rendering = true;
        }

        void Move() {
          if(currentGameState == Game_State::running && rendering == true) {
            if(canMove == true) {
              bool hitPlayer = false;
              int newX = posX;
              if(movesLeft == true) {
                newX -= 1;
                if(newX < 0) {
                  rendering = false;
                }
              } else {
                newX += 1;
                if(newX > 16) {
                  rendering = false;
                }
              } // end check move right
              if(newX <= ((*currentPlayer).posX + (*currentPlayer).size)) {
                hitPlayer = true;
                currentGameState = Game_State::over;
              } else {
                posX = newX;
              }
            } // end check can move
          } // end check game is running
        }
    };
}
