#include "MidiOutPort.h"
#include <showtime/ZstLogging.h>
#include <showtime/ZstConstants.h>
#include "MidiConstants.h"

using namespace showtime;

MidiOutPort::MidiOutPort(unsigned int port_number, const std::string& port_name) :
	showtime::ZstComponent(MIDIOUT_COMPONENT_TYPE, ("OUT_" + port_name).c_str()),
	m_note_on_plug(std::make_unique<showtime::ZstInputPlug>("send_noteon", ZstValueType::IntList)),
	m_note_off_plug(std::make_unique<showtime::ZstInputPlug>("send_noteoff", ZstValueType::IntList)),
	m_polyaftertouch_plug(std::make_unique<showtime::ZstInputPlug>("send_polyaftertouch", ZstValueType::IntList)),
	m_cc_plug(std::make_unique<showtime::ZstInputPlug>("send_controlchange", ZstValueType::IntList)),
	m_progchange_plug(std::make_unique<showtime::ZstInputPlug>("send_programchange", ZstValueType::IntList)),
	m_channelaftertouch_plug(std::make_unique<showtime::ZstInputPlug>("send_channelaftertouch", ZstValueType::IntList)),
	m_pitchbend_plug(std::make_unique<showtime::ZstInputPlug>("send_pitchbend", ZstValueType::IntList))
{
}

void MidiOutPort::on_registered()
{
	add_child(m_note_on_plug.get());
	add_child(m_note_off_plug.get());
	add_child(m_polyaftertouch_plug.get());
	add_child(m_cc_plug.get());
	add_child(m_progchange_plug.get());
	add_child(m_channelaftertouch_plug.get());
	add_child(m_pitchbend_plug.get());
}

void MidiOutPort::compute(showtime::ZstInputPlug* plug)
{
	if (plug == m_note_on_plug.get()) {
		if (plug->size() > 2 ) send_note_on(plug->int_at(0), plug->int_at(1), plug->int_at(2));
	} else if (plug == m_note_off_plug.get()) {
		if (plug->size() > 2) send_note_off(plug->int_at(0), plug->int_at(1), plug->int_at(2));
	} else if (plug == m_polyaftertouch_plug.get()) {
		if (plug->size() > 2) send_poly_aftertouch(plug->int_at(0), plug->int_at(1), plug->int_at(2));
	} else if (plug == m_cc_plug.get()) {
		if (plug->size() > 2) send_cc(plug->int_at(0), plug->int_at(1), plug->int_at(2));
	} else if (plug == m_progchange_plug.get()) {
		if (plug->size() > 1) send_program_change(plug->int_at(0), plug->int_at(1));
	} else if (plug == m_channelaftertouch_plug.get()) {
		if (plug->size() > 1) send_channel_aftertouch(plug->int_at(0), plug->int_at(1));
	} else if (plug == m_pitchbend_plug.get()) {
		if (plug->size() > 2) send_pitch_bend(plug->int_at(0), plug->int_at(1));
	}
}

void MidiOutPort::send_note_off(unsigned char note, unsigned char velocity, unsigned char channel)
{
	uint8_t midiDataBytes[4]{ NOTE_OFF, channel, note, velocity };
	send_midi(midiDataBytes, 4);
}

void MidiOutPort::send_note_on(unsigned char note, unsigned char velocity, unsigned char channel)
{
	uint8_t midiDataBytes[4]{ NOTE_ON, channel, note, velocity };
	send_midi(midiDataBytes, 4);
}

void MidiOutPort::send_poly_aftertouch(unsigned char note, unsigned char velocity, unsigned char channel)
{
	uint8_t midiDataBytes[4]{ NOTE_OFF, channel, note, velocity };
	send_midi(midiDataBytes, 4);
}

void MidiOutPort::send_cc(unsigned char controller, unsigned char value, unsigned char channel)
{
	uint8_t midiDataBytes[4]{ CONTROLLER_CHANGE, channel, controller, value };
	send_midi(midiDataBytes, 4);
}

void MidiOutPort::send_program_change(unsigned char program, unsigned char channel)
{
	uint8_t midiDataBytes[3]{ PROGRAM_CHANGE, channel, program };
	send_midi(midiDataBytes, 3);
}

void MidiOutPort::send_channel_aftertouch(unsigned char pressure, unsigned char channel)
{
	uint8_t midiDataBytes[3]{ CHANNEL_AFTERTOUCH, channel, pressure };
	send_midi(midiDataBytes, 3);
}

void MidiOutPort::send_pitch_bend(unsigned short value, unsigned char channel)
{
	uint8_t midiDataBytes[4]{ CHANNEL_AFTERTOUCH, channel, pitchbend_int_to_LSB(value), pitchbend_int_to_MSB(value) };
	send_midi(midiDataBytes, 4);
}
