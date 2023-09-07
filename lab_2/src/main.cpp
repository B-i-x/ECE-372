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
  initLEDs();  // set direction of LED pins
  //initialize switch
  initSwitch(); // set the switch for input and for pin change interrupts

  // initialize timer1
  initTimer1();  //initialize Timer 1 mode of operations using CTC mode

  // enable global interrupts
  sei();
  
  /*
  * Implement a state machine in the while loop which achieves the assignment
  * requirements.
  */
	while (1) {

    Serial.println("Starting timer");

    delayMs(5000);

    Serial.println("Ended Timer");

    switch (pbstate) {

      case wait_press:
        break;

      case debounce_press:
        delayMs(1);
        pbstate = wait_release;
        break;

      case wait_release:
        break;

      case debounce_release:
        delayMs(1);
        pbstate = wait_press;
        break;
      }


  }

  return 0;
}

/* Implement an Pin Change Interrupt which handles the switch being
* pressed and released. When the switch is pressed and released, the LEDs
* change at twice the original rate. If the LEDs are already changing at twice
* the original rate, it goes back to the original rate.
*/
// ISR for PCINT
// if the PCINT was triggered for press
