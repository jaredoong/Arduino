#include "Arduino.h"
#include <avr/interrupt.h>

unsigned int adcvalue, loval, hival;
unsigned int remapped_adc;

void InitPWM()
{
 TCNT0 = 0;
 OCR0A = 0;
 TCCR0A = 0b10000001;
 TIMSK0 |= 0b10;
}

void startPWM()
{
 TCCR0B = 0b00000100;
}

ISR(TIMER0_COMPA_vect)
{
  PORTB ^= (1 << PORTB5);
  OCR0A = remapped_adc;
}

ISR(ADC_vect)
{
  loval = ADCL;
  hival = ADCH;
  adcvalue = (hival << 8) | loval;
  remapped_adc = map(adcvalue,719,882, 0, 255); // range set based on measured value from flex sensor when straight and when bent
  ADCSRA |= (1 << ADSC);
}

void setup() {
 // put your setup code here, to run once:
 PRR &= 0b11111110;
 ADCSRA = 0b10001111;
 ADMUX = 0b01000000;
 DDRB |= 0b00100000;
 
 InitPWM();
 startPWM();
 sei();
 ADCSRA |= 0b01000000;
}

void loop(){}
