#include <Arduino.h>
#include <SevenSegmentDisplay.h>
#include <avr/io.h>
    

int main(){
  // Serial.begin(9600);
  
  initDisplay();
while (1==1){
  for (int i = 0; i < 10; i++) {
    WriteToDisplay(i);

    _delay_ms(500);

    // WriteToDisplay(-1);

    // _delay_ms(1000);

  }
}

  // for (int i = 0; i < 10; i++) {
  //   WriteToDisplay(i);

  //   

  // }
  
  
  return 0;
}

