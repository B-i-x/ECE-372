// this is the timer.cpp file for our own timer functions
// ECE 372A  9-17-20


#include <avr/io.h>
#include "timer.h"

// set up our timer for mode of operation 
// use CTC mode
// WGM bits need to be specified
// for CTC mode WGM00 = 0, WGM01 = 1, WGM02 = 0

void initTimer0(){
    TCCR0A &= ~( 1 << WGM00);
    TCCR0A |=  ( 1 << WGM01);
    TCCR0B &= ~( 1 << WGM02);
}

// This function passes an int value called delay 
//that will be used to specify how many milliseconds in the delay
void delayMs(int delay){

    int count = 0;

    // set up prescaler and output compare register value with formula
    // OCR0A = (Td * Clk Freq )/ (Prescaler)
    // Td = 1ms, clk freq = 16Mhz, Prescaler = 64
    // OCR0A = 249;
    //set the output compare register OCR0A to 249;

    OCR0A = 249;


    // TURN ON CLOCK , SET FREQ PRSCALER TO 64
    TCCR0B  |= (1 << CS00) | (1 << CS01);
    TCCR0B  &= ~ (1 << CS02);
    // go into the millisecond loop and loop until count = delay
    // after this while loop the delay time = delay * 1ms
    while (count < delay){
    // CLEAR THE OCF0A FLAG ()
    // CLEAR THE OCF0A flag by writing a 1 bit
    TIFR0 |= (1 << OCF0A);

    TCNT0 = 0;
    // CLEAR THE COUNTER TO RESTART COUNTING UP TO 249
    // WHILE THE FLAG OCF0A IS NOT RAISED DO NOTHING
    while ( ! (TIFR0 & ( 1 << OCF0A)));

        count++;
    }

    // turn clock off
    TCCR0B  &= ~ ((1 << CS00) | (1 << CS01) | ( 1 << CS02));

}