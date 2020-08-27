#include "RtMidiInPort.h"
#include "MidiConstants.h"
#include <showtime/ZstLogging.h>

using namespace showtime;

RtMidiInPort::RtMidiInPort(unsigned int port_index, const std::string& port_name) : MidiInPort(port_index, port_name)
{
	try {
		m_midiin = std::make_unique<RtMidiIn>();
		m_midiin->setCallback(s_midi_recv_callback, this);
		m_midiin->openPort(port_index, port_name);
	}
	catch (RtMidiError& error) {
		Log::app(Log::Level::error, "Midi In construction failed: {}", error.getMessage().c_str());
	}
}

void RtMidiInPort::s_midi_recv_callback(double timestamp, std::vector<unsigned char>* message, void* midi_in_port)
{
	auto port = static_cast<RtMidiInPort*>(midi_in_port);
	auto status = message->at(0);
	if (is_cc(&status)) {
		Log::app(Log::Level::debug, "Received CC");
	}
	// Do something
}
