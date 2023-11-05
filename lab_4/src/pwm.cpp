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

  //set header pin  to output
  DDRE |= (1 << DDE3);
  // set non-inverting mode - output starts high and then is low, 
  //COM1A0 bit = 0
  //COM1A1 bit =1
  TCCR3A |= (1 << COM3A1);
  TCCR3A &= ~(1 << COM3A0);

  //  Use fast PWM mode 10 bit, top value is determined by Table 17-2 of 0x3FF (1023) 
  //  which determines the PWM frequency.
  // for Fast PWM 10bit mode # 14:
  // WGM10 = 0
  // WGM11 = 1
  // WGM12 = 1
  // WGM13 = 1
  TCCR3A &= ~(1 << WGM30); 

  TCCR3A |= (1 << WGM31);

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
  OCR3A =  0;
}

void initPWMTimer4()  {

  //set header pin 6 to output
  DDRH |= (1 << DDH3);
  // set non-inverting mode - output starts high and then is low, 
  //COM1A0 bit = 1
  //COM1A1 bit = 1
  TCCR4A |= (1 << COM4A0);
  TCCR4A |= (1 << COM4A1);
    

  //  Use  PWM mode 10 bit, top value is determined by ICR1 value, 
  //  which determines the PWM frequency.
  // for mode 14:
  // WGM40 = 0
  // WGM41 = 1
  // WGM42 = 1
  // WGM43 = 1
  TCCR4A &= ~(1 << WGM40);
  TCCR4A |= (1 << WGM41); 
  TCCR4B |= (1 << WGM42);
  TCCR4B |= (1 << WGM43); 

  ICR4 = 1023;



  // set prescalar CSBits to prescaler of 1
  //CS30 =1
  //CS31 =0
  //CS32 =0
  TCCR4B |= (1 << CS41);
  TCCR4B &= ~((1 << CS40)  | (1 << CS42));

  OCR4A = 1023;

}

void changeDutyCycle(double dutycycle1, double dutycycle2){
  OCR3A = int(dutycycle1 * (1024));

  OCR4A = int(dutycycle2 * (1024));
}

void analyzeADC() {

  while(! ((1 << 4) & ADCSRA)) {//waiting for ADC to be ready
  }

  unsigned int adcResult = ADCL; //getting adc result p1
  adcResult += ((unsigned int) ADCH) << 8;  //getting adc result p2

  double percentage = adcResult / 1024.0; //getting the adc percentage
  double dc1, dc2;

  if (percentage < 0.5) {
    dc1 = 0.0;
    dc2 = (percentage * 2.0);
  }
  else {
    dc1 = ((percentage - 0.5) * 2.0);
    dc2 = 1.0; //for some reason, putting this at 100 makes it go to 0
  }

  changeDutyCycle(dc1, dc2);
  return;
}
