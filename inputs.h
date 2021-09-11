#pragma once

typedef void (*func_type)(void);

namespace inputs {
  struct Button {
        int pin = 0;
        // pressed is used because I want the fps of the render to not be insanely fast, but locked to a set number
        // that is because if it was as fast as the CPU goes, first of all it would render faster than we can see, unless I use delays
        // and secondly, it could also be open to random slow downs or break everything else due to those delays.
        // As such, we have two logic timelines, one that happens at 60FPS(FOR NOW) for the render, and another, which happens almost at CPU speed
        // for the input.
        // Once input is sent however, say u pressed the button, if you released it before the render loop occured, the player would have never jumped
        // so, once you press, I save it as a boolean, and I don't clear it until used in the render loop.
        bool pressed = false;
        bool held = false;
        bool released = false;
        //func_type functions[256];
        //void (*func)(void);
        func_type pressFunc;
        func_type holdFunc;
        func_type releaseFunc;
        
        Button(int startPin, func_type startPressFunc, func_type startHoldFund, func_type startReleaseFunc) {
            pin = startPin;
            pressFunc = startPressFunc;
            holdFunc = startHoldFund;
            releaseFunc = startReleaseFunc;
            //memcpy(functions, startFunctions, sizeof(startFunctions));
        }

        void Check() {
          if(analogRead(pin) >= 59 && analogRead(pin) < 200) {
            if(held == false) {
              if(pressed == false) {
                pressed = true;
              }
              held = true;
            }
          } else {
            if(held == true) {
              held = false;
              released=  true;
            }
          }
        }

        void OnRender() {
          Press();
          Hold();
          Release();
        }

        void Press() {
          if(pressed == true) {
            pressed = false;
            pressFunc();
            //for(int i = 0; i < 256; i++) {
            //  functions[i]();
            //}
          }
        }

        void Hold() {
          if(held == true) {
            holdFunc();
          }
        }

        void Release() {
          if(released == true) {
            releaseFunc();
            released = false;
          }
        }
  };
}
