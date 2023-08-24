// Author:         
// Net ID:         
// Date:           
// Assignment:     Lab 1
//----------------------------------------------------------------------//

#include <avr/io.h>

// initSwitch returns void and takes no parameters
void initSwitch(){
    DDRB &= ~(1<<DDA0); // set direction for input
    PORTA |= (1 << PORTA0);  // enable the pullup resistor for stable input
}
