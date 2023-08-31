// Author:         
// Net ID:         
// Date:           
// Assignment:     Lab 1
//----------------------------------------------------------------------//
#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>
#include "led.h"
#include "switch.h"

#define SHORT_DELAY 100
#define LONG_DELAY 1000

int main(){

  Serial.begin(9600);


  initLED();
  initSwitch();

  while(1){
    
    
    for (int i = 4; i < 11; i++) {
      //i dont go up to 11 because i = 4 and i = 11 are the same thing because match(4) == 11 and match(11) = 4


      if (i == 8) { continue;} //i = 7 and i =8 do the same thing because the match(7) = 8 and match(8) = 7

      Serial.println();
      Serial.println("On i:" + String(i));
      runLED(i);

      if(PINA & (1 << PINA0)){ // logic 1 true – turn off LED
        Serial.println("Push button ON");
        _delay_ms(LONG_DELAY);
        
      }
      else { // logic 0 - turn on LED
        Serial.println("OFF");
        _delay_ms(SHORT_DELAY);
      }

      Serial.flush();
    }
    

  }

  return 0;
}
