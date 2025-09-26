// Author: Alejandro Romero-Lozano
// Net ID: aromerolozano
// Date: 9-28-2023

#include <avr/io.h>
#include "timer.h"

// Normal mode (WGM02:0 = 0b000) so TCNT0 just free-runs to 0xFF and overflows.
void initTimer0(void) {
    // Clear WGM00, WGM01 in TCCR0A
    TCCR0A &= ~((1 << WGM00) | (1 << WGM01));
    // Clear WGM02 in TCCR0B
    TCCR0B &= ~(1 << WGM02);
}

/**
 * @brief Busy-wait delay using TCNT0 overflows.
 * 
 * With Fclk = 16 MHz and prescaler = 64:
 *  tick = 64 / 16e6 = 4 us
 *  1 ms = 1000 us = 250 ticks
 * Preload TCNT0 = 256 - 250 = 6 so the next overflow occurs in ~1 ms.
 */
void delayMs(int delay) {
    // Prescaler = 64 (CS01:CS00 = 1:1, CS02 = 0)
    TCCR0B |=  (1 << CS00) | (1 << CS01);
    TCCR0B &= ~(1 << CS02);

    unsigned int flagStatus = (TIFR0 & (1 << TOV0)) != 0;
    Serial.print("Overflow flag is: ");
    Serial.println(flagStatus);
    for (int i = 0; i < delay; i++) {
        // Clear any pending overflow flag by writing a 1
        TIFR0 |= (1 << TOV0);

        // Preload so overflow happens in ~1 ms (250 ticks)
        TCNT0 = 6;

        // Wait for overflow
        while (!(TIFR0 & (1 << TOV0))) {
            // spin
        }
        // (Flag auto-cleared by writing 1 above; if you want, you can clear again here.)
    }

    // Stop timer (clear all CS0x bits)
    TCCR0B &= ~((1 << CS00) | (1 << CS01) | (1 << CS02));
}
