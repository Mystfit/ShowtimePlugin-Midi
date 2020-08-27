#include "RtMidiBidirectionalPort.h"

RtMidiBidirectionalPort::RtMidiBidirectionalPort(const std::string& name, const std::string& in_port_name, int in_port_index, const std::string& out_port_name, int out_port_index, nlohmann::json map_data) :
	MidiBidirectionalPort(name, in_port_name, in_port_index, out_port_name, out_port_index, map_data),
	RtMidiInPort(in_port_index, in_port_name),
	RtMidiOutPort(out_port_index, out_port_name),
	ZstComponent(MIDI_BIDIRECTIONAL_COMPONENT_TYPE, name.c_str())
{
}

void RtMidiBidirectionalPort::send_midi(uint8_t* midibytes, size_t length)
{
	RtMidiOutPort::send_midi(midibytes, length);
}

void RtMidiBidirectionalPort::on_registered() { 
    RtMidiOutPort::on_registered();
    RtMidiInPort::on_registered();
}

void RtMidiBidirectionalPort::compute(showtime::ZstInputPlug* plug){
    RtMidiOutPort::compute(plug);
}

