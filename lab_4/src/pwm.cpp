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
  DDRB |= (1<<DDB7) | (1<<DDB6);

  //set header pin  to output
  DDRE |= (1 << DDE3);
  // set non-inverting mode - output starts high and then is low, 
  //COM1A0 bit = 0
  //COM1A1 bit =1
  TCCR3A |= (1 << COM3A1);
  TCCR3A &= ~(1 << COM3A0);

  //  Use fast PWM mode 10 bit, top value is determined by Table 17-2 of 0x3FF (1023) 
  //  which determines the PWM frequency.
  // for Fast PWM 10bit mode # 7:
  // WGM10 =1
  // WGM11 =1
  // WGM12 = 1
  // WGM13 = 0
  TCCR3A &= ~(1 << WGM30); 

  TCCR3A |=  (1 << WGM31);

  TCCR3B |= (1 << WGM32);
  TCCR3B |= (1 << WGM33);

  // PWM frequency calculation for FAST PWM mode on page 148 of datasheet
  //frequency of PWM = (F_clk)/((Prescaler)* (1 +TOP))
  // frequency of PWM = 16Mhz
  // Prescaler = 1
  // TOP value = 0x3FF = 1023 
  // PWM frequency from calculation = 15.625 kHz

ICR3 = 1023;

// set prescalar CSBits to prescaler of 1
//CS10 =1
//CS11 =0
//CS12 =0
TCCR3B |= (1 << CS31);
TCCR3B &= ~((1 << CS30)  | (1 << CS32));


// the last thing is to set the duty cycle.     
// duty cycle is set by dividing output compare OCR1A value by 1 + TOP value
// the top value is (1 + ICR1) = 1024
//  calculate OCR1A value => OCR1A = duty cycle(fractional number) * (1 + TOP) 
// we want a duty cycle = 60%
// OCR1A = 0.60 * 1024
 OCR3A =  512;
}

void initPWMTimer4()  {

  //set header pin 6 to output
  DDRG |= (1 << DDG5);
  // set non-inverting mode - output starts high and then is low, 
  //COM1A0 bit = 1
  //COM1A1 bit = 1
  TCCR4A |= (1 << COM4A0);
  TCCR4A |= (1 << COM4A1);
    

  //  Use  PWM mode 10 bit, top value is determined by ICR1 value, 
  //  which determines the PWM frequency.
  // for mode 10:
  // WGM30 =1
  // WGM31 =1
  // WGM32 = 1
  // WGM33 = 0
  TCCR4A &=  (1 << WGM40);
  TCCR4A |=  (1 << WGM41); 
  TCCR4B |=  (1 << WGM42);
  TCCR4B |= (1 << WGM43); 

  ICR4 = 1023;



  // set prescalar CSBits to prescaler of 1
  //CS30 =1
  //CS31 =0
  //CS32 =0
  TCCR4B |= (1 << CS41);
  TCCR4B &= ~((1 << CS40)  | (1 << CS42));

  OCR4A = 512;

}

void changeDutyCycle(float dutycycle1, float dutycycle2){
  OCR3A = int(dutycycle1 * (1024));

  OCR4A = int((dutycycle2) * (1024));
}

void analyzeADC(float voltage) {
  float dutycycle1, dutycycle2;



  if (voltage >= 2.4 && voltage <= 2.6) { //the 2.5 range
    // DDRB |= (1<<DDB7) | (1<<DDB6);
    PORTB &= ~(1<<PORTB7);
    PORTB &= ~(1<<PORTB6);
    // Serial.print("in 2.5v range, actual v:");

  }
  else if (voltage < 2.4) {
    PORTB &= ~(1<<PORTB6);
    PORTB |= (1<<PORTB7);
    dutycycle1 = 0;
    dutycycle2 = (voltage / 5.0);

    

    // Serial.print("in less than 2.5, actualv:");
  }
  else {
    PORTB |= (1<<PORTB6);
    PORTB &= ~(1<<PORTB7);

    dutycycle1 = (voltage / 5.0);
    dutycycle2 =0;

    // Serial.print("in more than 2.5, actualv:");
  }

  
  // Serial.print(voltage);
  changeDutyCycle(dutycycle1, dutycycle2);


  return;
}
