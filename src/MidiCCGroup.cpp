#include "MidiCCGroup.h"

using namespace showtime;
MidiCCGroup::MidiCCGroup(const std::string& name, unsigned int cc, ZstPlugDirection direction, MidiInPort* midi_in_port, MidiOutPort* midi_out_port) :
	ZstComponent(MIDI_CC_COMPONENT_TYPE, name.c_str()),
	m_midi_in_port(midi_in_port),
	m_midi_out_port(midi_out_port),
	m_cc_plug(cc)
{
	switch (direction) {
	case ZstPlugDirection::IN_JACK:
		m_input_plug = std::make_unique<ZstInputPlug>("send", ZstValueType::IntList);
		break;
	case ZstPlugDirection::OUT_JACK:
		m_output_plug = std::make_unique<ZstOutputPlug>("receive", ZstValueType::IntList);
		break;
	case ZstPlugDirection::NONE:
		m_input_plug = std::make_unique<ZstInputPlug>("send", ZstValueType::IntList);
		m_output_plug = std::make_unique<ZstOutputPlug>("receive", ZstValueType::IntList);
	}
}

void MidiCCGroup::on_registered()
{
	if(m_input_plug)
		add_child(m_input_plug.get());

	if(m_output_plug)
		add_child(m_output_plug.get());
}

void MidiCCGroup::compute(ZstInputPlug* plug)
{
	if (m_midi_out_port) {
		if (plug->size() > 1) {
			m_midi_out_port->send_cc(m_cc_plug, plug->int_at(0), plug->int_at(1));
		}
		else {
			m_midi_out_port->send_cc(m_cc_plug, plug->int_at(0));
		}
	}
}

MidiInPort* MidiCCGroup::midi_in_port()
{
	return m_midi_in_port;
}

MidiOutPort* MidiCCGroup::midi_out_port()
{
	return m_midi_out_port;
}
