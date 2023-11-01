#include <Arduino.h>
#include <SevenSegmentDisplay.h>

int main(){
  Serial.begin(9600);

  initDisplay();

  Serial.println("Winnning");

  for (int i = 0; i < 10; i++) {
    WriteToDisplay(i);

    _delay_ms(1000);
  }
  


  return 0;
}