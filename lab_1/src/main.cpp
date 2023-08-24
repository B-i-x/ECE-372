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

  while(1){
    
    for (int i = 4; i <= 11; i++) {
      turnOnLED(i);
      _delay_ms(LONG_DELAY);
      turnOffLED(i);
    }

  }

  return 0;
}
