#include <Arduino.h>
#include <SevenSegmentDisplay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "switch.h"
#include "timer.h"
#include "pwm.h"
#include "adc.h"

//Create states with enumaration type below, type name is button_state
typedef enum{wait_press,debounce_p,wait_release,debounce_r}button_state;

// Initialize states.  Remember to use volatile 
//Declare the enum state variable, wait_press is the home state
volatile button_state my_button_state = wait_press;

#define SHORT_DELAY 100 //100ms delay
#define LONG_DELAY 500 //200ms delay
#define DEBOUNCE_TIME 1 //Debounce time is 1ms
volatile unsigned int flip_speed = 0; // global variable to flip the toggle blinking speed
//volatile unsigned char num = 0x0; //global variable to loop through 0-15 in binary
    

int main(){
  Serial.begin(9600);
  sei();
  initDisplay();

  initTimer0();
  initTimer1();

  initSwitchPD0();

  initPWMTimer3();
  initPWMTimer4();
  initADC7();
  unsigned int result = 0;
  float voltage = 0;
  while(1){
    
    result = ADCL;
      result += ((unsigned int) ADCH) << 8;
      voltage = result * (4.98 /1024.0);
      // Serial.println(voltage,2);

    // changeDutyCycle(voltage);
    analyzeADC(voltage);

    // delayMs(300);
    // Serial.print(" calculated dutycyle is:");
    // Serial.println(dutyCycle, 2);

    // changeDutyCycle(dutyCycle);
    
    //state machine for push button
    switch (my_button_state){
      case wait_press: // Home state
        break;
      case debounce_p:
        delayMs(DEBOUNCE_TIME); //wait for button to be pushed 1ms before changing state
        my_button_state = wait_release; //go to wait_release
      case wait_release:
        break;
      case debounce_r:
        delayMs(DEBOUNCE_TIME); //wait for button to be released 1ms before changing state
        changeDutyCycle(0, 0);
        for (int i = 9; i > -1; i--) {
        WriteToDisplay(i);
        delayUs(1);
        }
        
        WriteToDisplay(10);

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