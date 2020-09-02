#include "MidiInPort.h"
#include "MidiConstants.h"
#include <showtime/ZstLogging.h>
#include <showtime/ZstConstants.h>

using namespace showtime;

MidiInPort::MidiInPort(unsigned int port_number, const std::string& port_name) :
	showtime::ZstComponent(MIDIIN_COMPONENT_TYPE, ("IN_" + port_name).c_str()),
	m_received_note_on(std::make_unique<showtime::ZstOutputPlug>("recv_noteon", ZstValueType::IntList)),
	m_received_note_off(std::make_unique<showtime::ZstOutputPlug>("recv_noteoff", ZstValueType::IntList)),
	m_received_cc(std::make_unique<showtime::ZstOutputPlug>("recv_cc", ZstValueType::IntList)),
	m_received_sysex(std::make_unique<showtime::ZstOutputPlug>("recv_sysex", ZstValueType::IntList))
{
}

void MidiInPort::on_registered()
{
	add_child(m_received_note_on.get());
	add_child(m_received_note_off.get());
	add_child(m_received_cc.get());
	add_child(m_received_sysex.get());
}

void MidiInPort::receive_note_on(const std::vector<unsigned char>& note_data, uint8_t channel)
{
	m_received_note_on->append_int(channel);
	m_received_note_on->append_int(note_data[0]);
	m_received_note_on->append_int(note_data[1]);
	m_received_note_on->fire();
}

void MidiInPort::receive_note_off(const std::vector<unsigned char>& note_data, uint8_t channel)
{
	m_received_note_off->append_int(channel);
	m_received_note_off->append_int(note_data[0]);
	m_received_note_off->append_int(note_data[1]);
	m_received_note_off->fire();
}

void MidiInPort::receive_cc(const std::vector<unsigned char>& note_data, uint8_t channel)
{
	m_received_cc->append_int(channel);
	m_received_cc->append_int(note_data[0]);
	m_received_cc->append_int(note_data[1]);
	m_received_cc->fire();
}

void MidiInPort::receive_sysex(const std::vector<unsigned char>& sysex_data, uint8_t channel)
{
	/*m_received_sysex->append_int(channel);
	m_received_sysex->append_int(sysex_data[0]);
	m_received_cc->fire();*/
}

void MidiInPort::receive_midi(uint8_t* mididata, size_t length)
{
	if (length < 3)
		return;

	auto status = status_from_status_byte(&mididata[0]);
	auto channel = channel_from_status_byte(&mididata[0]);
	std::vector<uint8_t> midi_data{ mididata[0], mididata[1] };

	if (is_note_on(&status)) {
		receive_note_on(midi_data, channel);
	}
	else if (is_cc) {
		receive_cc(midi_data, channel);
	}
	else if (is_note_off) {
		receive_note_off(midi_data, channel);
	}
}
