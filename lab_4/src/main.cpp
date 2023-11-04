#include <Arduino.h>
#include <avr/io.h>

#include <SevenSegmentDisplay.h>
#include <pwm.h>
#include <adc.h>

int main(){
  Serial.begin(9600);
  
  initDisplay();


  initPWMTimer3();
  initPWMTimer4();

  initADC0();

  while(1) {
    analyzeADC();

    // _delay_ms(100);
  }
  // while (1==1){
  // for (int i = 0; i < 10; i++) {
  //   WriteToDisplay(i);

  //   _delay_ms(500);

  //   // WriteToDisplay(-1);

  //   // _delay_ms(1000);

  // }
  // }

  // for (int i = 0; i < 10; i++) {
  //   WriteToDisplay(i);

  //   

  // }
  
  
  return 0;
}

