#pragma once

#include <RtMidi.h>
#include "MidiInPort.h"

class RtMidiInPort : public MidiInPort {
public:
	RtMidiInPort(unsigned int port_index, const std::string& port_name);
protected:

private:
	static void s_midi_recv_callback(double timestamp, std::vector<unsigned char>* message, void* midi_in_port);

	std::unique_ptr<RtMidiIn> m_midiin;
};
