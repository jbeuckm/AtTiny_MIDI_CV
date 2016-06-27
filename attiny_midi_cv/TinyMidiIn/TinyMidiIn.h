#ifndef Tiny_Midi_In_h
#define Tiny_Midi_In_h

#include "Arduino.h"

/*! Enumeration of MIDI types */
enum MidiType
{
    InvalidType           = 0x00,    ///< For notifying errors
    NoteOff               = 0x80,    ///< Note Off
    NoteOn                = 0x90,    ///< Note On
    PitchBend             = 0xE0,    ///< Pitch Bend
    NotesOff              = 0xA0,    ///< AllNotesOff
    SystemExclusive       = 0xF0,    ///< System Exclusive
    SystemReset           = 0xFF,    ///< System Real Time - System Reset
};


class TinyMidiIn {

    public:
        TinyMidiIn();

        void inputByte(byte midiByte);
    
        void setFilterChannel(byte channel);

        void setHandleNoteOff(void (*fptr)(byte channel, byte note, byte velocity));
        void setHandleNoteOn(void (*fptr)(byte channel, byte note, byte velocity));
        void setHandlePitchBend(void (*fptr)(byte channel, int bend));
        void setHandleSystemExclusive(void (*fptr)(byte * array, unsigned size));
        void setHandleSystemReset(void (*fptr)(void));

    private:
    
        byte nib1, nib2;
        byte filterChannel;

        byte messageType;
        byte messagePosition;
        byte data1;
        byte data2;
    
        void executeChannelMessage();
    
        void (*mNoteOffCallback)(byte channel, byte note, byte velocity);
        void (*mNoteOnCallback)(byte channel, byte note, byte velocity);
        void (*mPitchBendCallback)(byte channel, int);
        void (*mSystemExclusiveCallback)(byte * array, unsigned size);
        void (*mSystemResetCallback)(void);
};

#endif
