#pragma once

#include <showtime/ZstExports.h>
#include <showtime/entities/ZstComponent.h>
#include <showtime/entities/ZstPlug.h>
#include <stdint.h>

#define MIDIIN_COMPONENT_TYPE "midiinport"

class MidiInPort : 
	public virtual showtime::ZstComponent
{
public:
	ZST_PLUGIN_EXPORT MidiInPort(unsigned int port_number, const std::string& port_name);
	ZST_PLUGIN_EXPORT virtual void on_registered() override;

	ZST_PLUGIN_EXPORT virtual void receive_midi(uint8_t* mididata, size_t length);

protected:
	ZST_PLUGIN_EXPORT virtual void receive_note(const std::vector<unsigned char>& note_data);
	ZST_PLUGIN_EXPORT virtual void receive_cc(const std::vector<unsigned char>& note_data);
	ZST_PLUGIN_EXPORT virtual void receive_sysex(const std::vector<unsigned char>& sysex_data);

private:
	// Input plugs
	std::unique_ptr<showtime::ZstOutputPlug> m_received_note;
	std::unique_ptr<showtime::ZstOutputPlug> m_received_cc;
	std::unique_ptr<showtime::ZstOutputPlug> m_received_sysex;
};
