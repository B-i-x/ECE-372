// Author: 
//  Alex Romero
//  Victor Oviedo
//  Mason Marrero
//  Omar Ramos
// Date: 11/7
// Assignment: lab 4
//
// Description: main
//----------------------------------------------------------------------//
#include <Arduino.h>
#include <avr/io.h>

#include "SevenSegmentDisplay.h"
#include "pwm.h"
#include "adc.h"
#include "timer.h"
#include "switch.h"


#define DEBOUNCE_TIME 1 //Debounce time is 1ms

volatile unsigned int flip_speed = 0; // global variable to flip the toggle blinking speed
//volatile unsigned char num = 0x0; //global variable to loop through 0-15 in binary

//declarations
typedef enum{
  wait_press,
  debounce_p,
  wait_release,
  debounce_r
} button_state;

volatile button_state my_button_state = wait_press;

int main(){
  sei(); //enable intterupts

  // Serial.begin(9600);
  
  initDisplay();
  WriteToDisplay(-1);

  //acutal timers
  initTimer0(); //milliseconds
  initTimer1(); //seconds

  initPWMTimer3(); //pwm 1
  initPWMTimer4(); //pwm 2

  initADC0();

  initSwitchPD0(); //button
  while (1) {

    switch (my_button_state){

      case wait_press: // Home state
        analyzeADC();
        break;

      case debounce_p:
        delayMs(DEBOUNCE_TIME); //wait for button to be pushed 1ms before changing state
        my_button_state = wait_release; //go to wait_release
        break;

      case wait_release:
        break;

      case debounce_r:

        delayMs(DEBOUNCE_TIME); //wait for button to be released 1ms before changing state

        changeDutyCycle(0, 1);

        for (int i = 9; i > -1; i--) {
          WriteToDisplay(i);
          delayS(1);
        }
        
        WriteToDisplay(-1); //clear the display

        my_button_state = wait_press; //go to wait_press
        break;

      default:
        break;
    }	
  }
  
  return 0;
}



ISR(INT0_vect){ // Assign INT0_vect to work
  if (my_button_state == wait_press){ //when the pin change is detected, if in wait_press go to debounce_p
    my_button_state = debounce_p;
    }

  else if (my_button_state == wait_release){ //when the pin change is detected, if in wait_release go to debounce_r and change the flip_speed variable

    if (flip_speed == 0){ //change flip_speed to other value
      flip_speed = 1;
    }
    else{
      flip_speed = 0; //change flip_speed to original value
    }

    my_button_state = debounce_r;
    }
}