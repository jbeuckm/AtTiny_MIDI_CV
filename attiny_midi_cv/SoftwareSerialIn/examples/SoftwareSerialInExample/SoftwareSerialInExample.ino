/*
  SoftwareSerialIn test

 Receives from SoftwareSerialIn, sends to hardware serial.

 The circuit:
 * RX is digital pin 10 (connect to TX of other device)

 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts,
 so only the following can be used for RX:
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69

 Not all pins on the Leonardo support change interrupts,
 so only the following can be used for RX:
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).

 created back in the mists of time
 modified 25 May 2012
 by Tom Igoe
 based on Mikal Hart's example

 This example code is in the public domain.
 
 modified by Joe Beuckman to test receive only with SoftwareSerialIn

 */
#include <SoftwareSerialIn.h>

SoftwareSerialIn mySerial(10); // RX

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(57600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(4800);
}

void loop() // run over and over
{
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
}

