#include "TinyMIDI/TinyMIDI.h"
#include "SoftwareSerialIn/SoftwareSerialIn.h"
#include "AH_MCP4922.h"

#define GATE_PIN 3

AH_MCP4922 AnalogOutput1(10,11,12,LOW,LOW);
AH_MCP4922 AnalogOutput2(10,11,12,HIGH,LOW);

int liveNoteCount = 0;
int pitchbendOffset = 0;
int baseNoteFrequency;

SoftwareSerialIn mSerial(0);
MIDI_CREATE_INSTANCE(SoftwareSerialIn, mSerial, midiIn);

byte selectedChannel = 17;

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
  
  if (liveNoteCount == 0) {
    digitalWrite(GATE_PIN, HIGH);
  }
}


void handlePitchBend(byte channel, int bend)
{
  pitchbendOffset = bend >> 4;

  AnalogOutput1.setValue(baseNoteFrequency + pitchbendOffset);
}


// -----------------------------------------------------------------------------

void setup()
{
    int channelSpan = 1024 / 16;
    int channelInput = analogRead(0);
    selectedChannel = channelInput / channelSpan;
    
    pinMode(GATE_PIN, OUTPUT);
    digitalWrite(GATE_PIN, LOW);

    delay(1000);

    playScale(selectedChannel);

    // calibrate 8V
    baseNoteFrequency = (108 - 12) * 42;
    AnalogOutput1.setValue(baseNoteFrequency);
    // calibrate full velocity
    AnalogOutput2.setValue(32 * 127);

    midiIn.setHandleNoteOn(handleNoteOn);
    midiIn.setHandleNoteOff(handleNoteOff);
    midiIn.setHandlePitchBend(handlePitchBend);
    midiIn.begin();
}


void playScale(int channel) {

  int note = 60;

  for (int i=0; i<channel; i++) {

      handleNoteOn(channel, note, 100);
      delay(100);
      handleNoteOff(channel, note, 100);
      delay(100);
      note++;
  }

}


void loop()
{
    midiIn.read();
}

