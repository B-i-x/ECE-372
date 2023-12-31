// Author: Alejandro Romero-Lozano
// Net ID: aromerolozano
// Date: 10/13/2023
// Assignment:  Lab 3
// Description: This file implements the initialization of an external
// switch.
//----------------------------------------------------------------------//

#include "switch.h"
#include <avr/io.h>

/*
 * Initializes pull-up resistor on PB3 and sets it into input mode
 */
void initSwitchPB3(){
    DDRB &= ~(1<<DDB3); // set direction for input
    PORTB |= (1 << PORTB3);  // enable the pullup resistor for stable input

    // enable the group pin change interrupts PCINTs 0 through 7
    PCICR |= (1 << PCIE0);
    // enable the local PCINT 3 corresponding to development board header pin 12
    PCMSK0 |= (1 << PCINT3);
}
