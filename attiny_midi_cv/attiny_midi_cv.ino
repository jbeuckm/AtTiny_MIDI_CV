#include "SoftwareSerialIn/SoftwareSerialIn.h"
#include "TinyMidiIn/TinyMidiIn.h"
#include "AH_MCP4922.h"

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
  
  if (liveNoteCount == 0) {
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

    selectedChannel = 3;

    midiIn.setHandleNoteOn(handleNoteOn);
    midiIn.setHandleNoteOff(handleNoteOff);
    midiIn.setHandlePitchBend(handlePitchBend);
}


void loop() {
/*
  if (mSerial.available()) {
    
    byte midiByte = mSerial.read();
    
    digitalWrite(GATE_PIN,LOW);
    AnalogOutput1.setValue(midiByte);
    AnalogOutput2.setValue(midiByte);
    delay(1);
    
    digitalWrite(GATE_PIN,HIGH);
    AnalogOutput1.setValue(0);
    AnalogOutput2.setValue(0);
    delay(1);
  }
*/
  if (mSerial.available() != 0) {
    midiIn.inputByte(mSerial.read());
  }
}

/*


#include "SoftwareSerialIn/SoftwareSerialIn.h"
//#include "AH_MCP4922.h"

#define GATE_PIN 4

// AH_MCP4922( SDI, SCK, CS, DAC, GAIN)
//AH_MCP4922 AnalogOutput1(0,1,2,LOW,LOW);
//AH_MCP4922 AnalogOutput2(0,1,2,HIGH,LOW);

int liveNoteCount = 0;
int pitchbendOffset = 0;
int baseNoteFrequency;

SoftwareSerialIn mSerial(3);
MIDI_CREATE_INSTANCE(SoftwareSerialIn, mSerial, midiIn);

byte selectedChannel = 3;

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  
  liveNoteCount++;
  
  baseNoteFrequency = (pitch - 12) * 42;
//  AnalogOutput1.setValue(baseNoteFrequency + pitchbendOffset);
//  AnalogOutput2.setValue(velocity * 32);

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

//  AnalogOutput1.setValue(baseNoteFrequency + pitchbendOffset);
}


// -----------------------------------------------------------------------------

void setup()
{

    selectedChannel = 3;
    
    pinMode(GATE_PIN, OUTPUT);
    digitalWrite(GATE_PIN, HIGH);

    delay(1000);

    playScale(selectedChannel);

    // calibrate 8V
    baseNoteFrequency = (108 - 12) * 42;
//    AnalogOutput1.setValue(baseNoteFrequency);
    // calibrate full velocity
//    AnalogOutput2.setValue(32 * 127);

    midiIn.setHandleNoteOn(handleNoteOn);
    midiIn.setHandleNoteOff(handleNoteOff);
    midiIn.setHandlePitchBend(handlePitchBend);
    midiIn.begin(selectedChannel);

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
*/
