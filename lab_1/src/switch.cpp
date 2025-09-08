// Author: Alex Romero-Lozano 
// Net ID: aromerolozano
// Date: 08/31/2023  
// Assignment:     Lab 1
//----------------------------------------------------------------------//

#include <avr/io.h>

// initSwitch returns void and takes no parameters
void initSwitch(){
    DDRA &= ~(1<<DDA0); // set direction for input
    PORTA |= (1 << PORTA0);  // enable the pullup resistor for stable input
}
