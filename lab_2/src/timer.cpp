// Description: This file implements functions that utilize the timers
//----------------------------------------------------------------------//


#include <avr/io.h>
#include "timer.h"
/*
set up our timer0 for mode of operation 
use CTC mode
WGM bits need to be specified
for CTC mode 
    WGM00 = 0, 
    WGM01 = 1, 
    WGM02 = 0
*/


//void initTIMER0(){
//TCCR0A &= ~( 1 << WGM00);
//TCCR0A |=  ( 1 << WGM01);
//TCCR0B &= ~( 1 << WGM02);


// initialize mode of operation for Timer1 using CTC mode
// WGM bits need configured

void initTimer1() {
    TCCR1A &= ~( (1 << WGM10) | ( 1<< WGM11));
    TCCR1B |= ( 1 << WGM12);
    TCCR1B &= ~ ( 1 << WGM13); 
}
// function delayMS1 (delay)
// function takes an int value called delay to delay the total time in milliseconds
// this function is limited to delay = 1000 as the upper limit

void delayMs(int delay) {

    // set TCNT1 = 0
    TCNT1 = 0;

    // set outout compare value
    // we used the formula OCR1A = [Td * fclk]/ PS
    // using a PS = 256, Td = 1ms fclk = 16MHz.
    // OCR1A = 62
    // if we want to pass a value called delay then we can set OCR1A = 62 * delay
    // andthat should allow values in ms to be delayed by the right time up to 1000ms.

    OCR1A = 254 * delay;

    // set output compare flag down by writing a logic 1
    TIFR1 |= (1 << OCF1A);

    // turn on clock with the CS bits and start counting
    // Use Prescaleer of 256 (62 counts is approximately 1 ms)

    TCCR1B |= (1 << CS12);
    TCCR1B &= ~((1 << CS11)| (1 << CS10));

    // poll the flag OCF1A bit to see when it is raised
    // while the flag bit OCF1A is down , do nothing
    while (( TIFR1 & ( 1 << OCF1A)) == 0) ;

    // exit out we have our delay required
    // turn off clock
    TCCR1B &= ~( ( 1 << CS12) | ( 1 << CS11) | (1 << CS10));


}