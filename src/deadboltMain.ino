#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h> 

#define SS_PIN 10
#define RST_PIN 9
#define SERVO_PIN 8
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

byte allowedUIDs[5][4];
byte lastUid[4];
Servo servo; 

bool isLocked;

void setup() {
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 

  servo.attach(SERVO_PIN)

  setLock(false);
}

void loop() {
  sleep(1000)

  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent()) {
    return;
  }

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial()) {
    return;
  }

  printUid();

  if (isNewCard(rfid.uid.uidByte)) {
    if (isApprovedUid(rfid.uid.uidByte)) {
      setLock(!isLocked); // toggle the lock when activated
    } else {
      Serial.write("AAHH UNAPPROVED UID!!!")
    }
  } else {
    Serial.write("Card already read!");
  }

}

bool isNewCard(byte uid[]) {
  return !areUidsMatching(uid, lastUid);
}

bool isApprovedUid(byte uid[]) {
  for (int i=0; i< approvedUIDs.length; i++) {
    if (areUidsMatching(uid, approvedUIDs[i])) {
      return true;
    }
  }
  return false;
}

bool areUidsMatching(byte uid1[], byte uid2[]) {
  if (uid1.length != 4 || uid2.length != 4) {
    Serial.write("UID unexpected length!");
    return false;
  }
  return uid1[0] == uid2[0] &&
    uid1[1] == uid2[1] &&
    uid1[2] == uid2[2] &&
    uid1[3] == uid2[3];
}

void printUid() {
  Serial.println(F("The UID tag is:"));
  Serial.print(F("In hex: "));
  printHex(rfid.uid.uidByte, rfid.uid.size);
  Serial.println();
  Serial.print(F("In dec: "));
  printDec(rfid.uid.uidByte, rfid.uid.size);
  Serial.println();
}

// engageLock: Locks the door when true, unlocks it when false
void setLock(bool engageLock) {
  int position = 0;
  if (engageLock) {
    position = 180;
  }
  servo.write(position);
  isLocked = engageLock;
}

/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(' ');
    Serial.print(buffer[i], DEC);
  }
}