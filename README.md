# AtTiny85 MIDI-2-CV

Here is a little MIDI interface with a few, inexpensive parts on a single-sided PCB for DIY enthusiasts. 

An ATTINY85 microcontroller ($1.67) implements the SoftwareSerial and MIDI libraries, along with one to control a MCP4822 DAC for pitch and velocity CV outputs. A simple transistor switch enables a full-swing gate output. Pitch Bend messages smoothly change the pitch independent of the gate signal.

### Tuning

A trim pot is used to tune the pitch output for 1V/octave. At powerup, before any MIDI mesages are received, the pitch CV should be set to 8.0V.

### Pitch + CV Normals

The gate jack is normalled to the gate bus of the Eurorack connector. The pitch jack is normalled to the CV bus of the Eurorack connector.

### Set MIDI Channel

A six-byte System Exclusive message can be used to change the channel of the interface.

`F0` sysex start

`77 00 00` manufacturer + model + device

`03` channel (0 for omni) 

`F7` end of message


