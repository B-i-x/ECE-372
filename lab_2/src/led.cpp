// Description: This file implements turnOnLED and initLED to control
// the LEDs.
//----------------------------------------------------------------------//


#include <avr/io.h>
#include "led.h"


void initLED(){
    /* Initialize PA0, PA1, PA2, and PA3 to outputs
    */
   DDRA |= (1<<DDA0) | (1<<DDA1) | (1<<DDA3) | (1<<DDA3);
}


void turnOnLEDWithChar(unsigned char num){
    /* This must be one line of code.
    * In this function you will be giving a number "num" which will be represented
    * in binary by four LEDs. You must effectively assign the lowest four bits of
    * "num" to the appropriate bits of PORTA.
    */
   PORTA = (PORTA & 0xF0) | (num & 0x0F);

}
