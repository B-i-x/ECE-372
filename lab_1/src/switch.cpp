// Author:         
// Net ID:         
// Date:           
// Assignment:     Lab 1
//----------------------------------------------------------------------//

#include <avr/io.h>

// initSwitch returns void and takes no parameters
void initSwitch(){
 DDRB &= ~(1<<DDB6); // set direction for input
    PORTB |= (1 << PORTB6);  // enable the pullup resistor for stable input
}
