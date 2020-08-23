#include "RtMidiOutPort.h"
#include <showtime/ZstLogging.h>

using namespace showtime;

RtMidiOutPort::RtMidiOutPort(int port_number, const std::string& port_name) : MidiOutPort(port_number, port_name)
{
	try {
		m_midiout = std::make_unique<RtMidiOut>();
		m_midiout->openPort(port_number, port_name);
	}
	catch (RtMidiError& error) {
		Log::app(Log::Level::error, "Midi Out construction failed: {}", error.getMessage().c_str());
	}
}

void RtMidiOutPort::send_midi(uint8_t* midibytes, size_t length)
{
	if(m_midiout)
		m_midiout->sendMessage(midibytes, length);
}
