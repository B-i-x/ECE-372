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





#include <avr/io.h>
#include "led.h"
#include "switch.h"
#include "timer.h"
#include "lcd.h"

// defines


/*
 * Define a set of states that can be used in the state machine using an enum.
 */
// typedef enum .......


// Initialize states.  Remember to use volatile 




int main(){

  initTimer1();
  initLCD();
  initSwitchPB3();
  initLED();
  initTimer0();
  sei(); // Enable global interrupts.
  moveCursor(0, 0); // moves the cursor to 0,0 position
  writeString("Current mode: ");
  moveCursor(1, 0);  // moves the cursor to 1,0 position
  writeString("Fast");

// while loop
  
  return 0;
}

/* Implement an Pin Change Interrupt which handles the switch being
* pressed and released. When the switch is pressed and released, the LEDs
* change at twice the original rate. If the LEDs are already changing at twice
* the original rate, it goes back to the original rate.
*/
ISR(){

}