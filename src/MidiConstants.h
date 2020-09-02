// Adapted from https://spotlightkid.github.io/python-rtmidi/_modules/rtmidi/midiconstants.html

#pragma once
#include <stdint.h>
#include <math.h>

//"""Definitions of midi events, controller numbers and parameters."""

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Midi channel events(the most common events)
// Also called "Channel Voice Messages"

#define NOTE_OFF 0x80
// 1000cccc 0nnnnnnn 0vvvvvvv(channel, note, velocity)

#define NOTE_ON 0x90
// 1001cccc 0nnnnnnn 0vvvvvvv(channel, note, velocity)

#define POLY_AFTERTOUCH 0xA0
// 1010cccc 0nnnnnnn 0vvvvvvv(channel, note, velocity)

// see Channel Mode Messages for Controller Numbers
#define CONTROLLER_CHANGE 0xB0
// 1011cccc 0ccccccc 0vvvvvvv(channel, controller, value)

#define PROGRAM_CHANGE 0xC0
// 1100cccc 0ppppppp(channel, program)

#define CHANNEL_AFTERTOUCH 0xD0
// 1101cccc 0ppppppp(channel, pressure)

#define PITCH_BEND 0xE0
// 1110cccc 0vvvvvvv 0wwwwwww(channel, value - lo, value - hi)


//////////////////////////////////////////////////////////////////////////////////////////////////////
// Channel Mode Messages(Continuous Controller)
// All CCs have the same status byte(0xBn).
// The controller number is the first data byte

// High resolution continuous controllers(MSB)

#define BANK_SELECT_MSB 0x00
#define MODULATION_WHEEL_MSB 0x01
#define BREATH_CONTROLLER_MSB 0x02
#define FOOT_CONTROLLER_MSB 0x04
#define PORTAMENTO_TIME_MSB 0x05
#define DATA_ENTRY_MSB 0x06
#define CHANNEL_VOLUME_MSB 0x07
#define BALANCE_MSB 0x08
#define PAN_MSB 0x0A
#define EXPRESSION_CONTROLLER_MSB 0x0B
#define EFFECT_CONTROL_1_MSB 0x0C
#define EFFECT_CONTROL_2_MSB 0x0D
#define GENERAL_PURPOSE_CONTROLLER_1_MSB 0x10
#define GENERAL_PURPOSE_CONTROLLER_2_MSB 0x11
#define GENERAL_PURPOSE_CONTROLLER_3_MSB 0x12
#define GENERAL_PURPOSE_CONTROLLER_4_MSB 0x13

// High resolution continuous controllers(LSB)

#define BANK_SELECT_LSB 0x20
#define MODULATION_WHEEL_LSB 0x21
#define BREATH_CONTROLLER_LSB 0x22
#define FOOT_CONTROLLER_LSB 0x24
#define PORTAMENTO_TIME_LSB 0x25
#define DATA_ENTRY_LSB 0x26
#define CHANNEL_VOLUME_LSB 0x27
#define BALANCE_LSB 0x28
#define PAN_LSB 0x2A
#define EXPRESSION_CONTROLLER_LSB 0x2B
#define EFFECT_CONTROL_1_LSB 0x2C
#define EFFECT_CONTROL_2_LSB 0x2D
#define GENERAL_PURPOSE_CONTROLLER_1_LSB 0x30
#define GENERAL_PURPOSE_CONTROLLER_2_LSB 0x31
#define GENERAL_PURPOSE_CONTROLLER_3_LSB 0x32
#define GENERAL_PURPOSE_CONTROLLER_4_LSB 0x33

// Switches

// off: value <= 63, on : value >= 64
#define SUSTAIN_ONOFF 0x40
#define PORTAMENTO_ONOFF 0x41
#define SOSTENUTO_ONOFF 0x42
#define SOFT_PEDAL_ONOFF 0x43
#define LEGATO_ONOFF 0x44
#define HOLD_2_ONOFF 0x45

// Low resolution continuous controllers

// Sound Variation FX: Exciter On / Off
#define SOUND_CONTROLLER_1 0x46
// Harmonic Content FX: Compressor On / Off
#define SOUND_CONTROLLER_2 0x47
// Release Time FX: Distortion On / Off
#define SOUND_CONTROLLER_3 0x48
// Attack Time FX: EQ On / Off
#define SOUND_CONTROLLER_4 0x49
// Brightness FX: Expander On / Off
#define SOUND_CONTROLLER_5 0x4A
// Decay Time FX: Reverb On / Off
#define SOUND_CONTROLLER_6 0x4B
// Vibrato Rate FX: Delay On / Off
#define SOUND_CONTROLLER_7 0x4C
// Vibrato Depth FX: Pitch Transpose On / Off
#define SOUND_CONTROLLER_8 0x4D
// Vibrato Delay FX: Flange / Chorus On / Off
#define SOUND_CONTROLLER_9 0x4E
// Undefined FX: Special Effects On / Off
#define SOUND_CONTROLLER_10 0x4F
#define GENERAL_PURPOSE_CONTROLLER_5 0x50
#define GENERAL_PURPOSE_CONTROLLER_6 0x51
#define GENERAL_PURPOSE_CONTROLLER_7 0x52
#define GENERAL_PURPOSE_CONTROLLER_8 0x53
// PTC, 0vvvvvvv is the source Note number
#define PORTAMENTO_CONTROL 0x54
#define HIGH_RESOLUTION_VELOCITY_PREFIX 0x58
// Reverb Send Level, formerly Ext.Effects Depth
#define EFFECTS_1_DEPTH 0x5B
// formerly Tremelo Depth
#define EFFECTS_2_DEPTH 0x5C
// Chorus Send Level, formerly Chorus Depth
#define EFFECTS_3_DEPTH 0x5D
// formerly Celeste(Detune) Depth
#define EFFECTS_4_DEPTH 0x5E
// formerly Phaser Depth
#define EFFECTS_5_DEPTH 0x5F
// controller value byte should be 0
#define DATA_INCREMENT 0x60
// controller value byte should be 0
#define DATA_DECREMENT 0x61
#define NON_REGISTERED_PARAMETER_NUMBER_LSB 0x62
#define NON_REGISTERED_PARAMETER_NUMBER_MSB 0x63
#define REGISTERED_PARAMETER_NUMBER_LSB 0x64
#define REGISTERED_PARAMETER_NUMBER_MSB 0x65

// Channel Mode messages

// controller value byte should be 0
#define ALL_SOUND_OFF 0x78
// controller value byte should be 0
#define RESET_ALL_CONTROLLERS 0x79
// 0 off, 127 on
#define LOCAL_CONTROL_ONOFF 0x7A
// controller value byte should be 0
#define ALL_NOTES_OFF 0x7B
// controller value byte should be 0, also causes ANO
#define OMNI_MODE_OFF 0x7C
// controller value byte should be 0, also causes ANO
#define OMNI_MODE_ON 0x7D
// Mono Mode on / Poly Off also causes ANO
// 1011nnnn 01111110 0000vvvv
// vvvv > 0 : Number of channels to use(Omni Off).
// vvvv 0 : Use all available channels(Omni On)
#define MONO_MODE_ON 0x7E
// Poly Mode On / Mono Off
// controller value byte should be 0, also causes ANO
#define POLY_MODE_ON 0x7F


//////////////////////////////////////////////////////////////////////////////////////////////////////
// System Common Messages, for all channels

// 11110000 0iiiiiii 0ddddddd ... 11110111
#define SYSTEM_EXCLUSIVE 0xF0

// MIDI Time Code Quarter Frame
// 11110001
#define MIDI_TIME_CODE 0xF1

// 11110010 0vvvvvvv 0wwwwwww(lo - position, hi - position)
#define SONG_POSITION_POINTER 0xF2

// 11110011 0sssssss(songnumber)
#define SONG_SELECT 0xF3

// 11110100 (0xF4) is undefined
// 11110101 (0xF5) is undefined

// 11110110
#define TUNING_REQUEST 0xF6

// 11110111 // End of system exclusive
#define END_OF_EXCLUSIVE 0xF7


//////////////////////////////////////////////////////////////////////////////////////////////////////
// Midifile meta - events

#define MIDIFILE_SEQUENCE_NUMBER 0x00  // 00 02 ss ss(seq - number)
#define MIDIFILE_TEXT 0x01             // 01 len text...
#define MIDIFILE_COPYRIGHT 0x02     // 02 len text...
#define MIDIFILE_SEQUENCE_NAME 0x03   // 03 len text...
#define MIDIFILE_INSTRUMENT_NAME 0x04 // 04 len text...
#define MIDIFILE_LYRIC 0x05           // 05 len text...
#define MIDIFILE_MARKER 0x06           // 06 len text...
#define MIDIFILE_CUEPOINT 0x07         // 07 len text...
#define MIDIFILE_PROGRAM_NAME 0x08     // 08 len text...
#define MIDIFILE_DEVICE_NAME 0x09      // 09 len text...

#define MIDI_CH_PREFIX 0x20   // MIDI channel prefix assignment(deprecated)

#define MIDI_PORT 0x21     // 21 01 port, deprecated but still used
#define END_OF_TRACK 0x2F    // 2f 00
#define TEMPO 0x51        // 51 03 tt tt tt(tempo in µs / quarternote)
#define SMTP_OFFSET 0x54      // 54 05 hh mm ss ff xx
#define TIME_SIGNATURE 0x58   // 58 04 nn dd cc bb
#define KEY_SIGNATURE 0x59    // 59 02 sf mi(sf number of sharps(+) or flats(-)
 // mi major(0) or minor(1))
#define SPECIFIC 0x7F        // Sequencer specific event


//////////////////////////////////////////////////////////////////////////////////////////////////////
    // System Realtime messages
    // These should not occur in midi files

#define TIMING_CLOCK 0xF8
// 0xF9 is undefined
#define SONG_START 0xFA
#define SONG_CONTINUE 0xFB
#define SONG_STOP 0xFC
// 0xFD is undefined
#define ACTIVE_SENSING 0xFE
#define SYSTEM_RESET 0xFF


//////////////////////////////////////////////////////////////////////////////////////////////////////
    // META EVENT, it is used only in midi files.
    // In transmitted data it means system reset!!!

    // 11111111
#define META_EVENT 0xFF
#define ESCAPE_SEQUENCE 0xF7


//////////////////////////////////////////////////////////////////////////////////////////////////////
    // Misc constants

//#define FILE_HEADER 'MThd'
//#define TRACK_HEADER 'MTrk'

// Timecode resolution : frames per second
#define FPS_24 0xE8
#define FPS_25 0xE7
#define FPS_29 0xE3
#define FPS_30 0xE2


//////////////////////////////////////////////////////////////////////////////////////////////////////
    // Helper functions

inline bool is_status(uint8_t* byte) {
    //"""Return True if the given byte is a MIDI status byte, False otherwise."""
    return (*byte & 0x80) == 0x80;  // 1000 0000
}

inline bool is_note_on(uint8_t* byte) {
    return (*byte & NOTE_ON) == NOTE_ON;
}

inline bool is_note_off(uint8_t* byte) {
    return (*byte & NOTE_OFF) == NOTE_OFF;
}

inline bool is_cc(uint8_t* byte) {
    return (*byte & CONTROLLER_CHANGE) == CONTROLLER_CHANGE;
}

inline uint8_t status_from_status_byte(uint8_t* byte) {
    return (*byte | 0xF0);
}

inline uint8_t channel_from_status_byte(uint8_t* byte) {
    return (*byte | 0x0F);
}

inline uint8_t create_status_byte(uint8_t status, uint8_t channel) {
    return status | std::clamp((int)channel, 0, 15);
}

inline uint8_t pitchbend_int_to_MSB(uint16_t value) {
    return (value & 0x00FF) >> 8;
}

inline uint8_t pitchbend_int_to_LSB(uint16_t value) {
    return (value & 0x00FF);
}
