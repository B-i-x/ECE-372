// Author:         
// Net ID:         
// Date:          
// Assignment:     Lab 2
//
// Description: This file contains a programmatic overall description of the
// program. It should never contain assignments to special function registers
// for the exception key one-line code such as checking the state of the pin.
//
//----------------------------------------------------------------------//
#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "led.h"
#include "switch.h"
#include "timer.h"

#define SHORT_DELAY 250
#define LONG_DELAY 1000
/*
 * Define a set of states that can be used in the state machine using an enum.
 */
typedef enum stateType_enum {
  wait_press, 
  debounce_press, 
  wait_release,
  debounce_release
  }
  stateType;
  
// define state variable volatile
volatile stateType pbstate = wait_press;


volatile int led_speed = 2;


int main(){
  
  //serial for debugging
  Serial.begin(9600);

  //initialize LEDS
  // initLED();  // set direction of LED pins
  //initialize switch
  initSwitchPB3(); // set the switch for input and for pin change interrupts

  // initialize timer1
  initTimer0();  //initialize Timer 1 mode of operations using CTC mode

 
  // enable global interrupts
  sei();
  /*
  * Implement a state machine in the while loop which achieves the assignment
  * requirements.
  */

	while (1) {

    Serial.println("Start cycle");

    delayMs(1000);

    switch (pbstate) {

      case wait_press:
        Serial.println("In wait_press state");
        delayMs(500);

        break;

      case debounce_press:
        Serial.println("In debounce_press state");
        delayMs(500);

        pbstate = wait_release;
        break;

      case wait_release:
        Serial.println("In wait_release state");
        delayMs(500);

        break;

      case debounce_release:
        Serial.println("In debounce_release state");
        delayMs(500);

       
        pbstate = wait_press;
        break;
      }

    Serial.flush();
  }

  return 0;
}

/* Implement an Pin Change Interrupt which handles the switch being
* pressed and released. When the switch is pressed and released, the LEDs
* change at twice the original rate. If the LEDs are already changing at twice
* the original rate, it goes back to the original rate.
*/
// ISR for PCINT
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