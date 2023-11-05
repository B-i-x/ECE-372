#include "timer.h"
// Author: 
//  Alex Romero
//  Victor Oviedo
//  Mason Marrero
//  Omar Ramos
// Date: 11/7
// Assignment: lab 4
//
// Description: make a second and miliisecond timer
//----------------------------------------------------------------------//

void initTimer1(){
// intialize Timer1 for CTC mode of operation and interrupts
// WGM10 = 0, WGM11 = 0, WGM12 = 1, WGM13 = 0 for CTC mode
	TCCR1A &= ~(1 << WGM10);
    TCCR1A &= ~(1 << WGM11);
    TCCR1B |= (1 << WGM12); 
    TCCR1B &= ~(1 << WGM13);
    OCR1A = 15625; // set comparator value for CTC mode
}
/* This delays the program an amount of Seconds specified by unsigned int delay.
*/
void delayS(unsigned int delay){
    unsigned int counter = 0;
    // turn on the clock by setting the prescaler bits to 8
    TCCR1B &= ~(1 << CS01);
    TCCR1B |= (1 << CS02);
    TCCR1B |= (1 << CS00);

    while(counter < delay){
    // Put the output compare match flag down (OCF1A) by writing a logic 1
    TIFR1 |= (1 << OCF1A);
    TCNT1 = 0;  // CLEAR the count register

    // poll the OCF1A flag until it is set or is logic 1
    while (! (TIFR1 & (1 << OCF1A)));  // do nothing while the flag is down
    // after the flag has been raised 1 us time has elapsed
    counter++;
    } 

    // after exiting the larger while loop the time elasped has been 1us*delay
    // turn off the Timer1 clock
    TCCR1B &= ~((1 << CS00) | (1 << CS01));
    TCCR1B &= ~ (1 << CS02);
}


void initTimer0(){
// intialize Timer0 for CTC mode of operation and interrupts
// WGM00 = 0, WGM01 = 1, WGM02 = 0 for CTC mode
    TCCR0A &= ~(1 << WGM00);
    TCCR0A |=  (1 << WGM01);
    TCCR0B &= ~(1 << WGM02); 

    OCR0A = 249; // set comparator value for CTC mode
}

void delayMs(unsigned int delay){
   unsigned int counter = 0;
    // turn on the clock by setting the prescaler bits to 64
    TCCR0B |= (1 << CS00) | (1 << CS01);
    TCCR0B &= ~ (1 << CS02);

    while(counter < delay){
    // Put the output compare match flag down  (OCF0A) by writing a logic 1
    TIFR0 |= (1 << OCF0A);
    TCNT0 = 0;  // CLEAR the count register

    // poll the OCF0A flag until it is set or is logic 1
    while (! (TIFR0 & (1 << OCF0A)));  // do nothing while the flag is down
    // after the flag has been raised 1 ms time has elapsed
    counter++;
    } 

    // after exiting the larger while loop the time elasped has been 1ms*delay
    // turn off the Timer0 clock
    TCCR0B &= ~((1 << CS00) | (1 << CS01));
    TCCR0B &= ~ (1 << CS02);
}