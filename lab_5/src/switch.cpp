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

void initSwitchPD2(void) {
    cli();

    // D3 (PD3) input with pull-up
    DDRD  &= ~(1 << DDD2);
    PORTD |=  (1 << PORTD2);

    // Falling edge on INT1
    EICRA &= ~((1 << ISC01) | (1 << ISC00));
    EICRA |=  (1 << ISC01);        // ISC01=1, ISC00=0 -> falling edge

    EIFR  = (1 << INTF0);          // clear pending INT0
    EIMSK |= (1 << INT0);          // enable INT0

    // Make sure pin-change interrupts are off if you were using them
    PCMSK0 = 0;
    PCMSK1 = 0;
    PCMSK2 = 0;

    sei();
}

