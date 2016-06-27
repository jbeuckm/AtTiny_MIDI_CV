#include "TinyMidiIn.h"
#include "Arduino.h"

TinyMidiIn::TinyMidiIn()
{
    mNoteOffCallback                = 0;
    mNoteOnCallback                 = 0;
    mPitchBendCallback              = 0;
    mSystemExclusiveCallback        = 0;
    mSystemResetCallback            = 0;
}

void TinyMidiIn::inputByte(byte midiByte)
{
    if (mNoteOnCallback != 0) {
        mNoteOnCallback(1, 60, 100);
    }
}

void TinyMidiIn::setHandleNoteOff(void (*fptr)(byte channel, byte note, byte velocity)) { mNoteOffCallback = fptr; }
void TinyMidiIn::setHandleNoteOn(void (*fptr)(byte channel, byte note, byte velocity)) { mNoteOnCallback = fptr; }
void TinyMidiIn::setHandlePitchBend(void (*fptr)(byte channel, int bend)) { mPitchBendCallback = fptr; }
void TinyMidiIn::setHandleSystemExclusive(void (*fptr)(byte* array, unsigned size)) { mSystemExclusiveCallback = fptr; }
void TinyMidiIn::setHandleSystemReset(void (*fptr)(void)) { mSystemResetCallback = fptr; }