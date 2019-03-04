/**
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read data from more than one PICC to serial.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 *
 * Example sketch/program showing how to read data from more than one PICC (that is: a RFID Tag or Card) using a
 * MFRC522 based RFID Reader on the Arduino SPI interface.
 *
 * Warning: This may not work! Multiple devices at one SPI are difficult and cause many trouble!! Engineering skill
 *          and knowledge are required!
 *
 * @license Released into the public domain.
 *
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS 1    SDA(SS)      ** custom, take a unused pin, only HIGH/LOW required **
 * SPI SS 2    SDA(SS)      ** custom, take a unused pin, only HIGH/LOW required **
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 *
 */

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN           9          // Configurable, see typical pin layout above
#define SS_1_PIN          5          // Configurable, take a unused pin, only HIGH/LOW required
#define SS_2_PIN          6          // Configurable, take a unused pin, only HIGH/LOW required
#define SS_3_PIN          7          // Configurable, take a unused pin, only HIGH/LOW required
#define SS_4_PIN          8          // Configurable, take a unused pin, only HIGH/LOW required

#define NR_OF_READERS     4

#define cardDetected      2
#define cardSelect0       3 
#define cardSelect1       4

byte ssPins[4] = {SS_1_PIN, SS_2_PIN, SS_3_PIN, SS_4_PIN};

byte readCard[4];    // Stores scanned ID read from RFID Module
//byte masterCard[4] = {0xF3, 0x29, 0xEA, 0x21}; // Big Card
//byte masterCard[4] = {0x79, 0xA2, 0xC6, 0x02}; // Small Card
byte masterCard[4] = {0xCE, 0xF3, 0xE8, 0x62}; // Bahtiyar Bayram ID Card
boolean match = false;          // initialize card match to false
boolean production = false;

MFRC522 mfrc522[NR_OF_READERS];   // Create MFRC522 instance.

/**
 * Initialize.
 */
void setup() {
  pinMode(cardDetected, OUTPUT);
  pinMode(cardSelect0, OUTPUT);
  pinMode(cardSelect1, OUTPUT);
  
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);

  Serial.begin(9600); // Initialize serial communications with the PC
  //while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)

  SPI.begin();        // Init SPI bus

  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN); // Init each MFRC522 card
    Serial.print(F("Reader "));
    Serial.print(reader+1);
    Serial.print(F(": "));
    mfrc522[reader].PCD_DumpVersionToSerial();
  }
}

/**
 * Main loop.
 */
void loop() {

  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++) {
    // Look for new cards

    if(getID(reader)) {
      if(isMaster(readCard)) {
        if(!production) {
          Serial.println(".............Master.............");
          digitalWrite(10, HIGH);
        }
        digitalWrite(cardDetected, HIGH);
        delay(500);
        digitalWrite(cardDetected, LOW);
        if(!production) {
          digitalWrite(10, LOW);
        }
        switch(reader) {
          case 0:
            digitalWrite(cardSelect0, LOW);
            digitalWrite(cardSelect1, LOW);
            break;
          case 1:
            digitalWrite(cardSelect0, HIGH);
            digitalWrite(cardSelect1, LOW);
            break;
          case 2:
            digitalWrite(cardSelect0, LOW);
            digitalWrite(cardSelect1, HIGH);
            break;
          case 3:
            digitalWrite(cardSelect0, HIGH);
            digitalWrite(cardSelect1, HIGH);
            break;
          default:
            break;
        }
      }
    }
  }
}

/**
 * Helper routine to dump a byte array as hex values to Serial.
 */

///////////////////////////////////////// Get PICC's UID ///////////////////////////////////
int getID(uint8_t numOfReader) {
  // Getting ready for Reading PICCs
  if ( ! mfrc522[numOfReader].PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
    return 0;
  }
  if ( ! mfrc522[numOfReader].PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
    return 0;
  }
  // There are Mifare PICCs which have 4 byte or 7 byte UID care if you use 7 byte PICC
  // I think we should assume every PICC as they have 4 byte UID
  // Until we support 7 byte PICCs
  if(!production)
    Serial.println(F("Scanned PICC's UID:"));
  for (int i = 0; i < 4; i++) {  //
    readCard[i] = mfrc522[numOfReader].uid.uidByte[i];
    if(!production)
      Serial.print(readCard[i], HEX);
  }
  if(!production)
    Serial.println("");
  mfrc522[numOfReader].PICC_HaltA(); // Stop reading
  return 1;
}

///////////////////////////////////////// Check Bytes   ///////////////////////////////////
boolean checkTwo ( byte a[], byte b[] ) {
  if ( a[0] != '\0' )       // Make sure there is something in the array first
    match = true;       // Assume they match at first
  for ( int k = 0; k < 4; k++ ) {   // Loop 4 times
    if ( a[k] != b[k] )     // IF a != b then set match = false, one fails, all fail
      match = false;
  }
  if ( match ) {      // Check to see if if match is still true
    return true;      // Return true
  }
  else  {
    return false;       // Return false
  }
}

////////////////////// Check readCard IF is masterCard   ///////////////////////////////////
// Check to see if the ID passed is the master programing card
boolean isMaster( byte card[] ) {
  if ( checkTwo( card, masterCard ) )
    return true;
  else
    return false;
}
