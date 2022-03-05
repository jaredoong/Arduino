#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>

#define RST_PIN 9
#define SS_PIN 10
#define GREEN_LED 8

byte readCard[4];
String MasterTag = "438AEE11";  // REPLACE this Tag ID with your Tag ID!!!
String tagID = "";

// Create instances
MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal lcd(7, 6, 5, 4, 3, 2); //Parameters: (rs, enable, d4, d5, d6, d7) 

void setup() 
{
  // Initiating
  SPI.begin(); // SPI bus
  mfrc522.PCD_Init(); // MFRC522
  lcd.begin(16, 2); // LCD screen
  pinMode(GREEN_LED, OUTPUT);

  lcd.clear();
  lcd.print(" Access Control ");
  lcd.setCursor(0, 1);
  lcd.print("Scan Your Card>>");
}

void loop() 
{
  
  //Wait until new tag is available
  while (getID()) 
  {
    lcd.clear();
    digitalWrite(GREEN_LED, LOW);
    lcd.setCursor(0, 0);
    
    if (tagID == MasterTag) 
    {
      
      lcd.print(" Access Granted!");
      // You can write any code here like opening doors, switching on a relay, lighting up an LED, or anything else you can think of.
      digitalWrite(GREEN_LED, HIGH);
    }
    else
    {
      lcd.print(" Access Denied!");
    }
    
      lcd.setCursor(0, 1);
      lcd.print(" ID : ");
      lcd.print(tagID);
      
    delay(2000);

    // reset LCD
    lcd.clear();
    // reset LED
    digitalWrite(GREEN_LED, LOW);
    lcd.print(" Access Control ");
    lcd.setCursor(0, 1);
    lcd.print("Scan Your Card>>");
  }
}

//Read new tag if available
boolean getID() 
{
  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
  return false;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
  return false;
  }
  tagID = "";
  for ( uint8_t i = 0; i < 4; i++) { // The MIFARE PICCs that we use have 4 byte UID
  //readCard[i] = mfrc522.uid.uidByte[i];
  tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); // Adds the 4 bytes in a single String variable
  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); // Stop reading
  return true;
}
