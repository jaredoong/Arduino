#include "Arduino.h"

#define PIN4 (1 << 4)
#define PIN5 (1 << 5)

void setup() 
{
 // setting DDRB as output
 DDRB |= ((PIN4) | (PIN5)); // Set PB5 & PB4 as output
}

void loop() 
{
 // toggle both LED's one at a time
 PORTB = PIN5; //Green LED: ON, Red LED: OFF
 delay(1000);
 PORTB = PIN4; //Green LED: OFF, Red LED: ON
 delay(1000);
}
