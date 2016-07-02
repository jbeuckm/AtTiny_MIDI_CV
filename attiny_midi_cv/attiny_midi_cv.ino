#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <MIDI.h>
#include "AH_MCP4922.h"

#define ALL_NOTES_OFF 123
#define CTRL_RESET 121

#define GATE_PIN 4
#define MIDI_CHANNEL_ADDRESS 0

SoftwareSerial mSerial(3, 5);
MIDI_CREATE_INSTANCE(SoftwareSerial, mSerial, midiIn);

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


void playScale(int numNotes) {

  int note = 60;

  for (int i=0; i<numNotes; i++) {

      handleNoteOn(selectedChannel, note, 100);
      delay(100);
      handleNoteOff(selectedChannel, note, 100);
      delay(100);
      note++;
  }

}


void handleSystemExclusive(byte *message, unsigned size) {

  if (message[1] == 0x77) // manufacturer ID
  if (message[2] == 0)    // model ID
  if (message[3] == 0) {  // device ID
    selectedChannel = message[4] % 17;
    EEPROM.write(MIDI_CHANNEL_ADDRESS, selectedChannel);
    midiIn.begin(selectedChannel);
    
    playScale(selectedChannel);
  }
}


void handleControlChange(byte channel, byte number, byte value)
{
  switch (number) {

    case CTRL_RESET:
      handlePitchBend(selectedChannel, 0);
      break;
      
    case ALL_NOTES_OFF:
      liveNoteCount = 0;
      digitalWrite(GATE_PIN, HIGH);
      break;
  }
}

void setup() {
  OSCCAL += 3;

  pinMode(GATE_PIN, OUTPUT);
  digitalWrite(GATE_PIN, HIGH);

  selectedChannel = EEPROM.read(MIDI_CHANNEL_ADDRESS);
  if (selectedChannel > 16) {
    selectedChannel = 0;
    EEPROM.write(MIDI_CHANNEL_ADDRESS, selectedChannel);
  }

  midiIn.setHandleNoteOn(handleNoteOn);
  midiIn.setHandleNoteOff(handleNoteOff);
  midiIn.setHandlePitchBend(handlePitchBend);
  midiIn.setHandleSystemExclusive(handleSystemExclusive);
  midiIn.setHandleControlChange(handleControlChange);
  
  midiIn.begin(selectedChannel);
}

void loop() {
  midiIn.read();  
}

