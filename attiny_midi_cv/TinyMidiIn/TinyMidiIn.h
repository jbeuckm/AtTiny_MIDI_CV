#ifndef Tiny_Midi_In_h
#define Tiny_Midi_In_h

#include "Arduino.h"


class TinyMidiIn {

public:
    TinyMidiIn();

    void inputByte(byte midiByte);

    void setHandleNoteOff(void (*fptr)(byte channel, byte note, byte velocity));
    void setHandleNoteOn(void (*fptr)(byte channel, byte note, byte velocity));
    void setHandlePitchBend(void (*fptr)(byte channel, int bend));
    void setHandleSystemExclusive(void (*fptr)(byte * array, unsigned size));
    void setHandleSystemReset(void (*fptr)(void));

    private:
        void (*mNoteOffCallback)(byte channel, byte note, byte velocity);
    void (*mNoteOnCallback)(byte channel, byte note, byte velocity);
    void (*mPitchBendCallback)(byte channel, int);
    void (*mSystemExclusiveCallback)(byte * array, unsigned size);
    void (*mSystemResetCallback)(void);
};

#endif
