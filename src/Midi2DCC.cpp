#include "Midi2DCC.h"

Midi2DCC::Midi2DCC(const std::string& name, unsigned int x_cc, unsigned int y_cc, showtime::ZstPlugDirection direction, MidiInPort* midi_in_port, MidiOutPort* midi_out_port) :
	MidiCCGroup(name, -1, direction, midi_in_port, midi_out_port),
	m_x_cc(x_cc),
	m_y_cc(y_cc)
{
}

void Midi2DCC::compute(showtime::ZstInputPlug* plug)
{
	if (plug->size() == 2) {
		midi_out_port()->send_cc(m_x_cc, plug->int_at(0));
		midi_out_port()->send_cc(m_y_cc, plug->int_at(1));
	}
	else if(plug->size() == 3){
		midi_out_port()->send_cc(m_x_cc, plug->int_at(0), plug->int_at(2));
		midi_out_port()->send_cc(m_y_cc, plug->int_at(1), plug->int_at(2));
	}
}
