#include "MidiInPort.h"
#include "MidiConstants.h"
#include <showtime/ZstLogging.h>
#include <showtime/ZstConstants.h>

using namespace showtime;

MidiInPort::MidiInPort(unsigned int port_number, const std::string& port_name) :
	showtime::ZstComponent(MIDIIN_COMPONENT_TYPE, ("IN_" + port_name).c_str()),
	m_received_note(std::make_unique<showtime::ZstOutputPlug>("recv_note", ZstValueType::IntList)),
	m_received_cc(std::make_unique<showtime::ZstOutputPlug>("recv_cc", ZstValueType::IntList)),
	m_received_sysex(std::make_unique<showtime::ZstOutputPlug>("recv_sysex", ZstValueType::IntList))
{
}

void MidiInPort::on_registered()
{
	add_child(m_received_note.get());
	add_child(m_received_cc.get());
	add_child(m_received_sysex.get());
}

void MidiInPort::receive_note(const std::vector<unsigned char>& note_data)
{
	m_received_note->append_int(note_data[0]);
	m_received_note->append_int(note_data[1]);
	m_received_note->append_int(note_data[2]);
	m_received_note->append_int(note_data[3]);
	m_received_note->fire();
}

void MidiInPort::receive_cc(const std::vector<unsigned char>& note_data)
{
}

void MidiInPort::receive_sysex(const std::vector<unsigned char>& sysex_data)
{
}

void MidiInPort::receive_midi(uint8_t* mididata, size_t length)
{
	if (length < 3)
		return;

	auto status = mididata[0];
	if (status == NOTE_ON) {
		receive_note(std::vector<unsigned char>{mididata[0], mididata[1], mididata[2], mididata[3]});
	}
}
