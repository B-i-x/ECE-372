#include "switch.h"
// Author:
//  Alex Romero
//  Victor Oviedo
//  Mason Marrero
//  Omar Ramos
// Date: 11/7
// Assignment: lab 5
//
// Description: use PD2 as a button, an INT button
//----------------------------------------------------------------------//

#include <avr/io.h>
#include <avr/interrupt.h>

/*
 * Initializes pull-up resistor on PD2 and enables INT0
 * Trigger: falling edge (high-to-low when the button is pressed if wired to GND)
 */

void initSwitchJ1(void) {
    // D3 (PD3 / INT1) input with pull-up
    DDRJ  &= ~(1 << DDJ1);
    PORTJ |=  (1 << PORTJ1);

    PCICR |= (1 << PCIE1);    // Enable Pin Change Interrupts for PCINT7:0
    PCMSK1 |= (1 << PCINT10);  // Enable PCINT3 (PD3)
}


