#include "pwm.h"

#include <avr/io.h>
#include <Arduino.h>


// function that produces an output PWM signal with a variable frequency and duty cycle. For this example
// I am generating a PWM frequency of 1MHz with a 75% duty cycle.
// I will use Timer 1 in the fast PWM mode #14 in Table 17-2, where my TOP value is ICR1
// In this mode I have more flexibility is setting the top value which in turn
// gives me more flexibility in setting the PWM frequency (compared to a fixed TOP value in
// modes 5, 6, and 7 Fast PWM).
// I will have a prescaler of 1.  The calculation of ICR1 and OCR1A are shown below:
void initPWMTimer3()  {

 //set header pin 11 to output
DDRB |= (1 << DDB5);
  // set non-inverting mode - output starts high and then is low, 
  //COM3A0 bit = 0
  //COM3A1 bit =1
  TCCR3A |= (1 << COM3A1);
  TCCR3A &= ~(1 << COM3A0);

  //  Use fast PWM mode 10 bit, top value is determined by Table 17-2 of 0x3FF (1023) 
  //  which determines the PWM frequency.
// for Fast PWM 10bit mode # 7:
// WGM10 =1
// WGM11 =1
// WGM12 = 1
// WGM13 = 0
TCCR3A |=  (1 << WGM30) | (1 << WGM31);

TCCR3B |= (1 << WGM32);
TCCR3B &= ~(1 << WGM33); 

// PWM frequency calculation for FAST PWM mode on page 148 of datasheet
  //frequency of PWM = (F_clk)/((Prescaler)* (1 +TOP))
  // frequency of PWM = 16Mhz
  // Prescaler = 1
  // TOP value = 0x3FF = 1023 
  // PWM frequency from calculation = 15.625 kHz




// set prescalar CSBits to prescaler of 1
//CS10 =1
//CS11 =0
//CS12 =0
TCCR3B |= (1 << CS30);
TCCR3B &= ~((1 << CS31)  | (1 << CS32));


// the last thing is to set the duty cycle.     
// duty cycle is set by dividing output compare OCR1A value by 1 + TOP value
// the top value is (1 + ICR1) = 1024
//  calculate OCR1A value => OCR1A = duty cycle(fractional number) * (1 + TOP) 
// we want a duty cycle = 60%
// OCR1A = 0.60 * 1024
OCR3A =  614;

}

void changeDutyCycle(float dutycycle){
  OCR3A = int(dutycycle * (1 + 1024));

}

float analyzeADC(float voltage) {
  float dutycycle = 0;

  if (voltage >= 2.4 && voltage <= 2.6) { //the 2.5 range
    DDRB |= (1<<DDB7) | (1<<DDB6);
    PORTB &= (1<<PORTB7);
    PORTB &= ~(1<<PORTB6);
    Serial.print("in 2.5v range, actual v:");

  }
  else if (voltage < 2.4) {
    DDRB |= (1<<DDB7) | (1<<DDB6);
    PORTB |= (1<<PORTB6);
    PORTB &= ~(1<<PORTB7);

    dutycycle = 1 - (voltage / 2.4);

    Serial.print("in less than 2.5, actualv:");
  }
  else {
    DDRB |= (1<<DDB7) | (1<<DDB6);
    PORTB &= ~(1<<PORTB6);
    PORTB |= (1<<PORTB7);

    dutycycle = (voltage - 2.5) / 2.4;
    Serial.print("in more than 2.5, actualv:");
  }

  
  Serial.print(voltage);


  return dutycycle;
}
