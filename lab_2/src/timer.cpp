// Author: Alejandro Romero-Lozano        
// Net ID: aromerolozano
// Date: 9-28-2023 


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
/**
 * @brief Generates a delay in milliseconds using Timer0 on an AVR microcontroller.
 * 
 * This function configures Timer0 to generate a delay of the specified number of milliseconds.
 * It uses the Output Compare Match feature of Timer0 to achieve precise timing.
 * 
 * The Output Compare Register (OCR0A) value is calculated using the formula:
 * 
 *     OCR0A = (Td * Fclk) / Prescaler - 1
 * 
 * Where:
 * - Td = Desired delay time (1 ms in this case)
 * - Fclk = Clock frequency (16 MHz for the microcontroller)
 * - Prescaler = Timer prescaler value (64 in this case)
 * 
 * Substituting the values:
 * 
 *     OCR0A = (1e-3 * 16e6) / 64 - 1
 *     OCR0A = 250 - 1
 *     OCR0A = 249
 * 
 * @param delay The number of milliseconds to delay.
 */
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