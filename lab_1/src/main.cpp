// Author:         
// Net ID:         
// Date:           
// Assignment:     Lab 1
//----------------------------------------------------------------------//

#include <avr/io.h>
#include <util/delay.h>
#include "led.h"
#include "switch.h"

#define SHORT_DELAY 100
#define LONG_DELAY 1000

int main(){

  initLED();
  initSwitch();

  while(1){
    
    
    for (int i = 4; i <= 11; i++) {

      runLED(i);

      if(PINB & (1 << PINB6)){ // logic 1 true – turn off LED
        _delay_ms(LONG_DELAY);
      }
      else { // logic 0 - turn on LED
        _delay_ms(SHORT_DELAY);
      }

    }

  }

  return 0;
}
