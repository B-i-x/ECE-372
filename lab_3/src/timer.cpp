// Author: Alejandro Romero-Lozano
// Net ID: aromerolozano
// Date: 10/13/2023
// Assignment: Lab 3
//----------------------------------------------------------------------//
#include <Arduino.h>

#include "timer.h"
//You many use any timer you wish for the microsecond delay and the millisecond delay


/* Initialize timer 1, you should not turn the timer on here. Use CTC mode  .*/
void initTimer1(){
    // initialize mode of operation for Timer1 using CTC mode
    // WGM bits need configured
    TCCR1A &= ~( (1 << WGM10) | ( 1<< WGM11));
    TCCR1B |= ( 1 << WGM12);
    TCCR1B &= ~ ( 1 << WGM13);
    //this is the microsecond delay
}

// /* This delays the program an amount of microseconds specified by unsigned int delay.
// */
void delayUs(unsigned int delay){
    unsigned int count = 0;

    // set outout compare value
    // we used the formula OCR0A = [Td * fclk]/ PS
    // using a PS = 8, Td = 1us fclk = 16MHz.
    // OCR1A = 2  - 1 to acccount for starting at bit 0.
    OCR1A = 1;

    // turn on clock with the CS bits and start counting
    // Use Prescaleer of 0 (16 counts is 1 us)
    TCCR1B |= (1 << CS11);
    TCCR1B &= ~((1 << CS10)| (1 << CS12));

    // poll the flag OCF1A bit to see when it is raised
    // while the flag bit OCF1A is down , do nothing
    while (count < delay){
        // CLEAR THE OCF0A FLAG ()
        // CLEAR THE OCF0A flag by writing a 1 bit
        TIFR1 |= (1 << OCF1A);

        TCNT1 = 0;
        // CLEAR THE COUNTER TO RESTART COUNTING UP TO 249
        // WHILE THE FLAG OCF0A IS NOT RAISED DO NOTHING
        while(!(TIFR1 & ( 1 << OCF1A))){
        }
        // Serial.println("Works?");

        count++;
    }

    // exit out we have our delay required
    // turn off clock
    TCCR1B &= ~( ( 1 << CS12) | ( 1 << CS11) | (1 << CS10));
}

/* Initialize timer 0, you should not turn the timer on here.
* You will need to use CTC mode */
void initTimer0(){
    // WGM bits need to be specified
    // for CTC mode WGM00 = 0, WGM01 = 1, WGM02 = 0
	TCCR0A &= ~( 1 << WGM00);
    TCCR0A |=  ( 1 << WGM01);
    TCCR0B &= ~( 1 << WGM02);
    //this is the millisecond delay

}
/* This delays the program an amount specified by unsigned int delay.
* Use timer 0. Keep in mind that you need to choose your prescalar wisely
* such that your timer is precise to 1 millisecond and can be used for
* 100-2000 milliseconds
*/
void delayMs(unsigned int delay){
    unsigned int count = 0;

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
        while(!(TIFR0 & ( 1 << OCF0A))){}

        count++;
    }

    // turn clock off
    TCCR0B  &= ~ ((1 << CS00) | (1 << CS01) | ( 1 << CS02));
}