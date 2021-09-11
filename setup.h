#pragma once
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#define byte uint8_t

// TIMING

unsigned long RENDER_INTERVAL = 42;
unsigned long LOGIC_INTERVAL = 2;
unsigned long OBSTACLE_MOVEMENT_INTERVAL = 506;
unsigned long MULTIPLIER_CHANGE_INTERVAL = 5000;
float OBSTACLE_MOVEMENT_MULTIPLIER_CHANGER = 0.15f;

float OBSTACLE_MAX_MOVEMENT_MULTIPLER = 0.70f;
unsigned long OBSTACLE_SPAWN_INTERVAL = OBSTACLE_MOVEMENT_INTERVAL * 3;

unsigned long last_render_time = 0;
unsigned long last_logic_time = 0;
unsigned long last_obstacle_movement_time = 0;
float obstacle_movement_multiplier = 1;
unsigned long last_multiplier_change = 0;

// PINS
int BTN_PIN_ONE = 0;

int isRunning = 0;

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

byte obstacleTop[8] = {
    0b11111,
    0b01111,
    0b00110,
    0b00110,
    0b00111,
    0b01111,
    0b11110,
    0b00000
  };
  
  byte obstacleBottom[8] = {
    0b00000,
    0b11110,
    0b01111,
    0b00111,
    0b00110,
    0b00110,
    0b01111,
    0b11111
  };

  byte sprite[24] = {
    // TAIL
    0b00000,
    0b00100,
    0b00110,
    0b00111,
    0b00011,
    0b00001,
    0b00000,
    0b00000,
    // RUN ONE
    0b11111,
    0b11011,
    0b11111,
    0b11100,
    0b11110,
    0b11100,
    0b10100,
    0b10100,
    // RUN TWO
    0b11111,
    0b11011,
    0b11111,
    0b11100,
    0b11110,
    0b11100,
    0b10100,
    0b10100
  };

  const int tailIndex = 0;
  const int runOneIndex = 1;
  const int runTwoIndex = 2;
  const int obstacleTopIndex = 3;
  const int obstacleBottomIndex = 4;

  // 999999999
  int score = 0;
  unsigned long SCORING_INTERVAL = 1000;
  unsigned long last_score_time = 0;
  int MAX_SCORE = 999999;
  
  enum class Game_State
  {
      stopped,
      running,
      over
  };
  
  Game_State currentGameState = Game_State::stopped;
