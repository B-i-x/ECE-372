// Author: Alejandro Romero-Lozano
// Date: 9-29-2025
// Timer0 init for 1 ms CTC tick (ISR is implemented in main.cpp)

#include <avr/io.h>
#include "timer.h"

void timer0_init_1ms_ctc(void) {
  // CTC mode: WGM01=1, WGM00=0, WGM02=0
  TCCR0A &= ~(1 << WGM00);
  TCCR0A |=  (1 << WGM01);
  TCCR0B &= ~(1 << WGM02);

  // 1 ms @ 16 MHz, prescaler 64 => OCR0A = (16e6/64)*0.001 - 1 = 249
  OCR0A = 249;

  // Clear counter
  TCNT0 = 0;

  // Enable Compare Match A interrupt
  TIMSK0 |= (1 << OCIE0A);

  // Start timer: prescaler 64 (CS01=1, CS00=1; CS02=0)
  TCCR0B |=  (1 << CS01) | (1 << CS00);
  TCCR0B &= ~(1 << CS02);
}
