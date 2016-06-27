#include "SoftwareSerialIn/SoftwareSerialIn.h"
#include "TinyMidiIn/TinyMidiIn.h"
#include "AH_MCP4922.h"
#include <EEPROM.h>

SoftwareSerialIn mSerial(3);
TinyMidiIn midiIn = TinyMidiIn();

#define GATE_PIN 4

// AH_MCP4922( SDI, SCK, CS, DAC, GAIN)
AH_MCP4922 AnalogOutput1(0,1,2,LOW,LOW);
AH_MCP4922 AnalogOutput2(0,1,2,HIGH,LOW);

byte selectedChannel = 3;

int liveNoteCount = 0;
int pitchbendOffset = 0;
int baseNoteFrequency;

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  liveNoteCount++;
  
  baseNoteFrequency = (pitch - 12) * 42;
  AnalogOutput1.setValue(baseNoteFrequency + pitchbendOffset);
  AnalogOutput2.setValue(velocity * 32);

  digitalWrite(GATE_PIN, LOW);
 }

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  liveNoteCount--;
  
  if (liveNoteCount <= 0) {
    digitalWrite(GATE_PIN, HIGH);
  }
}

void handlePitchBend(byte channel, int bend)
{
  pitchbendOffset = bend >> 4;
  AnalogOutput1.setValue(baseNoteFrequency + pitchbendOffset);
}


void setup() {
    
    pinMode(3, INPUT);
    mSerial.begin(31250);
    
    pinMode(GATE_PIN, OUTPUT);
    digitalWrite(GATE_PIN, HIGH);

    selectedChannel = EEPROM.read(0);
    if (selectedChannel > 15) {
      selectedChannel = 0;
      EEPROM.write(0, selectedChannel);
    }
    midiIn.setFilterChannel(selectedChannel);

    midiIn.setHandleNoteOn(handleNoteOn);
    midiIn.setHandleNoteOff(handleNoteOff);
    midiIn.setHandlePitchBend(handlePitchBend);
}

void loop() {
  if (mSerial.available() != 0) {
    midiIn.inputByte(mSerial.read());
  }
}

