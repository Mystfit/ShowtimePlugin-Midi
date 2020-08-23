#pragma once

#include <RtMidi.h>
#include "MidiOutPort.h"

class RtMidiOutPort : public MidiOutPort {
public:
	RtMidiOutPort(int port_number, const std::string& port_name);
protected:
	void send_midi(uint8_t* midibytes, size_t length) override;

private:
	// Output midi port
	std::unique_ptr<RtMidiOut> m_midiout;
};
