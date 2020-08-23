#include "MidiCCGroup.h"

using namespace showtime;
MidiCCGroup::MidiCCGroup(const std::string& name, unsigned int cc) :
	ZstComponent(MIDI_CC_COMPONENT_TYPE, name.c_str()),
	m_cc_plug(cc),
	m_input_plug(std::make_unique<ZstInputPlug>("in", ZstValueType::IntList)),
	m_output_plug(std::make_unique<ZstOutputPlug>("out", ZstValueType::IntList))
{
}

void MidiCCGroup::on_registered()
{
	add_child(m_input_plug.get());
	add_child(m_output_plug.get());
}

void MidiCCGroup::compute(ZstInputPlug* plug)
{
}
