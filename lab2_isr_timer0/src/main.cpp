// Author: Alejandro Romero-Lozano
// Date: 9-29-2025
// Blink LEDs from the Timer0 Compare Match ISR. Button on PB3 (PCINT3) uses the
// original 4-state machine for debouncing and toggles blink speed on release.

#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "led.h"
#include "switch.h"
#include "timer.h"  // void timer0_init_1ms_ctc(void);

// ---------------- Config ----------------
#define SHORT_DELAY_MS 100
#define LONG_DELAY_MS  200
#define DEBOUNCE_MS     50   // debounce window used by the state machine

/*
 * Define the same state machine as before.
 */
typedef enum stateType_enum {
  wait_press,
  debounce_press,
  wait_release,
  debounce_release
} stateType;

// ---------------- Globals (shared with ISRs) ----------------
volatile stateType pbstate = wait_press;
volatile uint8_t  led_speed = 2;          // 2 = slow (LONG), 1 = fast (SHORT) like your original
volatile uint32_t g_ms = 0;               // 1 ms system tick from TIMER0_COMPA
volatile uint16_t blink_accum = 0;        // accumulates ms toward next LED step
volatile uint8_t  led_index = 0;          // which LED pattern index to show
volatile uint32_t debounce_start_ms = 0;  // timestamp when entering a debounce state

// --------------- Small atomic helper ----------------
static inline uint32_t millis_atomic() {
  uint32_t snap;
  uint8_t s = SREG; cli();
  snap = g_ms;
  SREG = s;
  return snap;
}

// ---------------- Entry ----------------
int main() {
  // Optional: Serial.begin(9600);

  initLED();         // set LED pins as outputs
  initSwitchPB3();   // configure PB3 with pull-up + PCINT enable (as in your project)
  timer0_init_1ms_ctc();  // 1 ms tick; enables OCIE0A

  sei();  // global interrupts on

  // The timer ISR handles blinking. Here we just run the original state machine for debounce.
  while (1) {
    switch (pbstate) {
      case wait_press:
        // Idle until a PCINT puts us into debounce_press
        break;

      case debounce_press: {
        // After DEBOUNCE_MS, confirm the button is still PRESSED (active low) then go to wait_release
        uint32_t now = millis_atomic();
        if ((now - debounce_start_ms) >= DEBOUNCE_MS) {
          // confirm PB3 still low (pressed)
          uint8_t pressed = (((PINB >> PB3) & 0x01) == 0);
          if (pressed) {
            pbstate = wait_release;
          } else {
            // bounce reverted; go back to waiting for a press
            pbstate = wait_press;
          }
        }
        break;
      }

      case wait_release:
        // Idle until a PCINT puts us into debounce_release (we already toggled speed there)
        break;

      case debounce_release: {
        // After DEBOUNCE_MS, confirm the button is still RELEASED (high) then go to wait_press
        uint32_t now = millis_atomic();
        if ((now - debounce_start_ms) >= DEBOUNCE_MS) {
          uint8_t released = (((PINB >> PB3) & 0x01) == 1);
          pbstate = released ? wait_press : wait_release;
        }
        break;
      }
    }
    // Nothing time-sensitive in the loop; ISRs do the heavy lifting.
  }

  return 0;
}

// ---------------- ISRs (both live in this file, per your request) ----------------

// Timer0 Compare Match A -> 1 ms system tick + LED blink scheduler
ISR(TIMER0_COMPA_vect) {
  g_ms++;

  // Determine blink period from led_speed (match your original semantics)
  const uint16_t period = (led_speed == 2) ? LONG_DELAY_MS : SHORT_DELAY_MS;

  if (++blink_accum >= period) {
    blink_accum = 0;

    // Advance LED pattern (keep this very lightweight — just PORT writes)
    turnOnLEDWithChar(led_index);
    // Wrap to your pattern length. If you have 8 states, this works:
    led_index = (uint8_t)((led_index + 1) & 0x07);
    // If your pattern length differs, adjust the wrap accordingly.
  }
}

/*
 * Pin Change Interrupt on PORTB (PB3 is PCINT3).
 * We keep the **original state machine**:
 *  - From wait_press -> debounce_press (on first edge)
 *  - From wait_release -> (toggle speed) -> debounce_release
 * The actual "debounce delay and confirm" is handled back in the main loop.
 */
ISR(PCINT0_vect) {
  // Read current PB3 level (pull-up: pressed=0, released=1)
  const uint8_t pin = (PINB >> PB3) & 0x01;

  if (pbstate == wait_press) {
    // Any edge here is treated as start of a press; start debounce window
    debounce_start_ms = g_ms;
    pbstate = debounce_press;
  } else if (pbstate == wait_release) {
    // On first edge heading toward release:
    // Toggle speed immediately (like your original), then debounce the release
    if (led_speed == 2) {
      led_speed = 1;
    } else {
      led_speed = 2;
    }
    debounce_start_ms = g_ms;
    pbstate = debounce_release;
  }
  // If we're already in a debounce_* state, ignore further pin changes
  // until the main loop confirms after DEBOUNCE_MS.
}
