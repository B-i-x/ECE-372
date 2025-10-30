#include "pwm.h"
// Author: 
//  Alex Romero
//  Victor Oviedo
//  Mason Marrero
//  Omar Ramos
// Date: 11/7
// Assignment: lab 4
//
// Description: get two PWM timers and function to analyze ADC
//----------------------------------------------------------------------//


// function that produces an output PWM signal with a variable frequency and duty cycle. For this example
// I am generating a PWM frequency of 1MHz with a 75% duty cycle.
// I will use Timer 1 in the fast PWM mode #14 in Table 17-2, where my TOP value is ICR1
// In this mode I have more flexibility is setting the top value which in turn
// gives me more flexibility in setting the PWM frequency (compared to a fixed TOP value in
// modes 5, 6, and 7 Fast PWM).
// I will have a prescaler of 1.  The calculation of ICR1 and OCR1A are shown below:
// pwm.cpp
#include <avr/io.h>

#define PWM_TOP 1023

static inline uint16_t clamp_to_top(float x){
  if (x < 0.0f) x = 0.0f;
  if (x > 1.0f) x = 1.0f;
  // scale to 0..1023 with rounding
  return (uint16_t)(x * PWM_TOP + 0.5f);
}

void initPWMTimer3(void){
  // OC3A = PE3, OC3B = PE4 on ATmega2560
  DDRE |= (1 << DDE3) | (1 << DDE4);

  // Reset control regs
  TCCR3A = 0;
  TCCR3B = 0;

  // Fast PWM mode 14: TOP = ICR3
  // WGM33:WGM30 = 1110b
  TCCR3A |= (1 << WGM31);
  TCCR3B |= (1 << WGM32) | (1 << WGM33);

  // Non-inverting on OC3A and OC3B
  TCCR3A |= (1 << COM3A1);
  TCCR3A &= ~(1 << COM3A0);

  TCCR3A |= (1 << COM3B1);
  TCCR3A &= ~(1 << COM3B0);

  ICR3  = PWM_TOP;
  OCR3A = 0;
  OCR3B = 0;

  // Prescaler = 1
  TCCR3B |= (1 << CS30);
}

void changeDutyCycle(float dutyA, float dutyB){
  OCR3A = clamp_to_top(dutyA);
  OCR3B = clamp_to_top(dutyB);
}

// Motor helpers, assuming OC3A drives one side and OC3B the other.
// Only PWM one leg at a time for direction control.
void motorSet(float speed, int dir){
  if (speed < 0) speed = 0;
  if (speed > 1) speed = 1;

  if (dir >= 0){
    // Forward: A PWM, B low
    OCR3A = clamp_to_top(speed);
    OCR3B = 0;
  } else {
    // Reverse: B PWM, A low
    OCR3A = 0;
    OCR3B = clamp_to_top(speed);
  }
}

// Active braking: both legs to the same rail
void motorBrakeLow(void){    // brake to GND
  OCR3A = 0;
  OCR3B = 0;
}

void motorBrakeHigh(void){   // brake to VCC
  OCR3A = PWM_TOP;
  OCR3B = PWM_TOP;
}

// If your driver has an EN pin, pulling EN low is the true "coast"


void analyzeADC(void) {
  // start conversion
  ADCSRA |= (1 << ADSC);
  // wait for complete
  while ((ADCSRA & (1 << ADIF)) == 0) {}
  // read result (L then H)
  uint16_t adc = ADCL;
  adc |= ((uint16_t)ADCH) << 8;
  // clear ADIF by writing 1
  ADCSRA |= (1 << ADIF);

  float pct = adc / 1023.0f;

  float dc1, dc2;
  if (adc < 512) {
    // direction A: PWM OC3A, keep OC4A low
    dc1 = (pct * 2.0f);   // 0..~1
    dc2 = 0.0f;
  } else {
    // direction B: PWM OC4A, keep OC3A low
    dc1 = 0.0f;
    dc2 = ((pct - 0.5f) * 2.0f);  // 0..~1
  }
  changeDutyCycle(dc1, dc2);
}
