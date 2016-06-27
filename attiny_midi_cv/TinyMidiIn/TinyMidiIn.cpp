#include "TinyMidiIn.h"
#include "Arduino.h"

TinyMidiIn::TinyMidiIn()
{
    mNoteOffCallback                = 0;
    mNoteOnCallback                 = 0;
    mPitchBendCallback              = 0;
    mSystemExclusiveCallback        = 0;
    mSystemResetCallback            = 0;
    
    messagePosition = 0;
}

void TinyMidiIn::setFilterChannel(byte channel)
{
    filterChannel = channel;
}

void TinyMidiIn::inputByte(byte midiByte)
{
    if (messagePosition == 0) {
        
        nib1 = midiByte & B11110000;
        nib2 = midiByte & B00001111;
        
        switch (nib1) {
                
            case 0xF0:
                break;
                
            case NoteOn:
            case NoteOff:
            case PitchBend:
                messageType = nib1;
                messagePosition++;
                break;
                
            default:
                messageType = 0;
                break;
        }

    } 
    else if (messagePosition == 1) {
        
        switch (messageType) {
            case NoteOn:
            case NoteOff:
            case PitchBend:
                data1 = midiByte;
                messagePosition++;
                break;
                
            default:
                messagePosition = 0;
                break;
        }
    }
    else if (messagePosition == 2) {
        
        switch (messageType) {
            case NoteOn:
            case NoteOff:
            case PitchBend:
                data2 = midiByte;
                executeChannelMessage();
                messagePosition = 0;
                break;
                
            default:
                messagePosition = 0;
                break;
        }
    }
    
}


void TinyMidiIn::executeChannelMessage()
{
    switch (messageType) {

        case NoteOn:
            if (mNoteOnCallback != 0) {
                mNoteOnCallback(filterChannel, data1, data2);
            }
            break;
            
        case NoteOff:
            if (mNoteOffCallback != 0) {
                mNoteOffCallback(filterChannel, data1, data2);
            }
            break;
            
        case PitchBend:
            if (mPitchBendCallback != 0) {
                mPitchBendCallback(filterChannel, ((int)data1 << 8) + (int)data2);
            }
            break;
    }
}


void TinyMidiIn::setHandleNoteOff(void (*fptr)(byte channel, byte note, byte velocity)) { mNoteOffCallback = fptr; }
void TinyMidiIn::setHandleNoteOn(void (*fptr)(byte channel, byte note, byte velocity)) { mNoteOnCallback = fptr; }
void TinyMidiIn::setHandlePitchBend(void (*fptr)(byte channel, int bend)) { mPitchBendCallback = fptr; }
void TinyMidiIn::setHandleSystemExclusive(void (*fptr)(byte* array, unsigned size)) { mSystemExclusiveCallback = fptr; }
void TinyMidiIn::setHandleSystemReset(void (*fptr)(void)) { mSystemResetCallback = fptr; }