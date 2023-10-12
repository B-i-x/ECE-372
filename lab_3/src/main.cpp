// Author:
// Net ID:
// Date:
// Assignment:     Lab 3
//
// Description: This file contains a programmatic overall description of the
// program. It should never contain assignments to special function registers
// for the exception key one-line code such as checking the state of the pin.
//
// Requirements:
//----------------------------------------------------------------------//
#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "led.h"
#include "switch.h"
#include "timer.h"
#include "lcd.h"

#define SHORT_DELAY 100
#define LONG_DELAY 200

typedef enum stateType_enum {
  wait_press, 
  debounce_press, 
  wait_release,
  debounce_release
} stateType;

// Initialize states.
// define state variable volatile
volatile stateType pbstate = wait_press;

volatile int led_speed = 2;



int main(){
  sei(); // Enable global interrupts.

  Serial.begin(9600);

  initTimer0();
  initTimer1();


  initSwitchPB3();

  initLED();
  int led_count = 0;


  initLCD();
  
  moveCursor(0, 0); // moves the cursor to 0,0 position
  writeString("Current mode: ");
  moveCursor(1, 0);  // moves the cursor to 1,0 position
  writeString("Fast");

  

	while (1) {
    

    if (led_speed == 2) {
      delayMs(LONG_DELAY);

      moveCursor(1, 0);  // moves the cursor to 1,0 position
      writeString("Slow ");

    }
    else {
      delayMs(SHORT_DELAY);
      moveCursor(1, 0);  // moves the cursor to 1,0 position
      writeString("Fast");
    }

    turnOnLEDWithChar(led_count);


    switch (pbstate) {

      case wait_press:
        // Serial.println("In wait_press state");
        delayMs(1);
        break;

      case debounce_press:
        // Serial.println("In debounce_press state");
        delayMs(1);
        pbstate = wait_release;
        break;

      case wait_release:
        // Serial.println("In wait_release state");
        delayMs(1);
        break;


      case debounce_release:
        // Serial.println("In debounce_release state");
        delayMs(1);
        pbstate = wait_press;
        break;
      }

    // Serial.flush();

    led_count++;
  }
  
  return 0;
}

/* Implement an Pin Change Interrupt which handles the switch being
* pressed and released. When the switch is pressed and released, the LEDs
* change at twice the original rate. If the LEDs are already changing at twice
* the original rate, it goes back to the original rate.
*/
// ISR for PCINT0
ISR(PCINT0_vect){
  // if the interrupt was triggered when state was waiting for press then we are going
  // debounce the press action  so set pbstate to debounce press
  if( pbstate == wait_press) {
    pbstate = debounce_press;
  }
  else if (pbstate == wait_release) {
    // else if interrupt was triggered when we were in wait_release, then we are going to 
    // debounce the release action so set the pbstate to debounce release but first
    // check the ledspeed and change it
    if (led_speed == 2) {  // if the led_speed is fast then change it to slow
      led_speed = 1;
    }
    else {
      led_speed = 2; // else led_speed was slow so change it to fast
    }
    
    pbstate = debounce_release;
  }  
}