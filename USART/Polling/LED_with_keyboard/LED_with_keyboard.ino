/* 
 *  Pressing the buttons will print either a '1' or '2' to the Serial Monitor
 *  Typing a '1' or '2' into the top line of Serial Monitor will determine which LED lights up
 *  Change 'Newline" to "No line ending" at bottom of Serial Monitor for input to be read correctly
 */

#include <avr/io.h>
#include <avr/interrupt.h>


// Masks for pins 12 and 11
#define PIN12MASK   0b00010000
#define PIN11MASK   0b00001000

static volatile char flashWhich=1;
static volatile char buttonVal=1;



void sendData(const char data)
{
  // Poll UDRE0 bit in UCSR0A until it is 1. When UDRE0 (bit 5) is 1, it is empty and hence ready to be written to
  while ((UCSR0A & 0b00100000) == 0);
  // Send data
  UDR0 = data;
  
}

char recvData()
{
  // If RXC0 bit is 0, return 0
  if ((UCSR0A & 0b10000000) == 0)
    return 0;
  // Otherwise return contents of UDR0 - '0'
  return UDR0 - '0';
}

void setupSerial()
{
  // Set up for 9600 8N1 for testing with Serial Monitor
  // Change to 9600 7E1 when communicating with another
  // Arduino
  UCSR0C = 0b00000110; // 9600 8N1
  UBRR0L = 103;
  UBRR0H = 0;
  UCSR0A = 0;
}

void startSerial()
{
  // Start the transmitter and receiver, but disable
  // all interrupts.
  UCSR0B = 0b00011000;
}

// Enable external interrupts 0 and 1
void setupEINT()
{
  // Configure INT0 and INT1 for rising edge triggered.
  // Remember to enable INT0 and INT1 interrupts.
  //DDRB |= 0b00011000;
  EICRA |= 0b00001111;
  EIMSK |= 0b00000011;
  sei();
}

// ISRs for external interrupts
ISR(INT0_vect)
{
  //Serial.println("1");
  buttonVal=1;  
  sendData('0'+buttonVal);
}

ISR(INT1_vect)
{
  //Serial.println("2");
  buttonVal=2;
  sendData('0'+buttonVal);
}

// Red is on pin 12
void flashRed()
{
    PORTB |= PIN12MASK;
    delay(100);
    PORTB &= ~PIN12MASK;
    delay(500);
}

// Green is on pin 11
void flashGreen()
{
    PORTB |= PIN11MASK;
    delay(100);
    PORTB &= ~PIN11MASK;
    delay(500);
}

void setup() {

  cli();
  // put your setup code here, to run once:

  //Serial.begin(9600);
  DDRB |= (PIN11MASK | PIN12MASK);
  setupEINT();
  setupSerial();
  startSerial();
  sei();
}

void loop() {
  // put your main code here, to run repeatedly:
  char data = recvData();

  if(data != 0) {
    //sendData(data);
    flashWhich = data;
  }
    
  if(flashWhich == 1)
    flashRed();
  else
    flashGreen();
}
