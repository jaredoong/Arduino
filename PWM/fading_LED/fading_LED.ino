#include "Arduino.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define PIN7 (1 << 7)
#define PIN6 (1 << 6)
#define PIN5 (1 << 5)
#define PIN4 (1 << 4)
#define PIN3 (1 << 3)
#define PIN2 (1 << 2)
#define PIN1 (1 << 1)
#define PIN0 (1 << 0)

static volatile int state = 1; // 1 when counting up, 0 when counting down

void setup() {
  TCNT0 = 0;
  TCCR0A = 0b10000001; // Set OCOM0A to 10 and WGM to 01
  TIMSK0 |= 0b10; // Enable Int for Output Compare Match
  OCR0A = 125; 
  TCCR0B = 0b00000100; // Set clk source to clk/256
                       // Changing the prescalar changes how fast the LED fades in and out
  //Set PORTD Pin 6 (Arduino Pin 6) as Output
  DDRD |= PIN6; 
  sei();
 }

// Interrupt used to change OCR0A, in order to change PWM duty cycle which in turn changes brightness of LED
ISR(TIMER0_COMPA_vect)
{
  if (OCR0A == 255) { // Reached max analog value, switch to count down
    state = 0;
  }
  else if (OCR0A == 0) { // Reached min analog value, switch to count up
     state = 1;
  }
  if (state == 1) { // Count up for Fade in effect
    OCR0A += 1;
  }
  else if (state == 0) { // Count down for Fade out effect
    OCR0A -= 1;
  }
}

void loop() {
  while (1) 
  {
  }
}
