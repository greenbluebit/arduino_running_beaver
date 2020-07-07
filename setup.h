#pragma once
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#define byte uint8_t

// TIMING

unsigned long RENDER_INTERVAL = 16.6;
unsigned long LOGIC_INTERVAL = 2;

unsigned long last_render_time = 0;
unsigned long last_logic_time = 0;

// PINS
int BTN_PIN_ONE = 6;

int isRunning = 0;

LiquidCrystal lcd(9, 8, 5, 4, 3, 2);

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
    0b10000,
    0b10000,
    // RUN TWO
    0b11111,
    0b11011,
    0b11111,
    0b11100,
    0b11110,
    0b11100,
    0b00100,
    0b00100
  };

  const int tailIndex = 0;
  const int runOneIndex = 1;
  const int runTwoIndex = 2;
  const int obstacleTopIndex = 3;
  const int obstacleBottomIndex = 4;

  int score = 0;
  
  enum class Game_State
  {
      stopped,
      running,
      over
  };
  
  Game_State currentGameState = Game_State::stopped;
