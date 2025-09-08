// Author: Alex Romero-Lozano 
// Net ID: aromerolozano
// Date: 08/31/2023  
// Assignment:     Lab 1
//----------------------------------------------------------------------//
#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>
#include "led.h"

// initLED which returns void and takes no parameters
void initLED() {
  // Digital 4 → PG5
  DDRG |= (1 << PG5);

  // Digital 5 → PE3
  DDRE |= (1 << PE3);

  // Digital 6–8 → PH3–PH5
  DDRH |= (1 << PH3) | (1 << PH4) | (1 << PH5);

  // Digital 9 → PH6
  DDRH |= (1 << PH6);

  // Digital 10–11 → PB4–PB5
  DDRB |= (1 << PB4) | (1 << PB5);
}


// turnOnLED which returns void and takes in an unsigned int parameter called led
void turnOnLED(unsigned int led){
    switch (led)
    {
    case 4:  // header pin 4
        PORTG |= ( 1 << PORTG5);
        break;

    case 5:  // header pin 5
        PORTE |= (1 << PORTE3);
        break;

    case 6:  // header piin 6
        PORTH |= (1 << PORTH3);
        break;
    
    case 7:  // header piin 7
        PORTH |= (1 << PORTH4);
        break;

    case 8:  // header piin 8
        PORTH |= (1 << PORTH5);
        break;

    case 9:  // header piin 9
        PORTH |= (1 << PORTH6);
        break;

    case 10:  // header piin 10
        PORTB |= (1 << PORTB4);
        break;

    case 11:  // header piin 11
        PORTB |= (1 << PORTB5);
        break;

    default:
        break;
    }
}

// turnOffLED which returns void and takes in an unsigned int parameter called led
void turnOffLED(unsigned int led){
    switch (led)
    {
    case 4:  // header pin 11
        PORTG &= ~(1 << PORTG5);
        break;

    case 5:  // header pin 12
        PORTE &= ~(1 << PORTE3);
        break;

    case 6:  // header piin 13
        PORTH &= ~(1 << PORTH3);
        break;
    
    case 7:  // header piin 13
        PORTH &= ~(1 << PORTH4);
        break;

    case 8:  // header piin 13
        PORTH &= ~(1 << PORTH5);
        break;

    case 9:  // header piin 13
        PORTH &= ~(1 << PORTH6);
        break;

    case 10:  // header piin 13
        PORTB &= ~(1 << PORTB4);
        break;

    case 11:  // header piin 13
        PORTB &= ~(1 << PORTB5);
        break;

    default:
        break;
    }

}

// runLED which returns void and takes in an unsigned int parameter called led
void runLED(unsigned int led){

    //the other led that u need to turn on is just the 15 - input
    /*
     ON      OFF
    4->11   5->10 (hardcoded)
    5->10   4->11
    6->9    5->10
    7->8    6->9
    8-x 8 is skipped in the for loop
    9->6    8->7
    10->5   9->6
    */
    int match = (-2 * led) + 15 + led;
    //same as match = 15 - led;

    Serial.println("Turning on LEDS: " + String(led) + " and " + String(match));
    turnOnLED(led);
    turnOnLED(match);

    if (led != 4) {
        //turn off previous  set of leds
        //if led is #4, then there is no previous to turn off so do nothing
        Serial.println("Turning off LEDS: " + String(led - 1) + " and " + String(match + 1));

        turnOffLED(led - 1);
        turnOffLED(match + 1);

    }
    else {
        turnOffLED(5);
        turnOffLED(10);
        /*
        Since we are not going up to i = 11, just remember to turn off the the old lights that were on when the for loop in main resets
        the old lights in the previous for loop are 5 and 10
        */

    }
  
}
