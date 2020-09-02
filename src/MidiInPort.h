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
	ZST_PLUGIN_EXPORT virtual void receive_note_on(const std::vector<unsigned char>& note_data, uint8_t channel);
	ZST_PLUGIN_EXPORT virtual void receive_note_off(const std::vector<unsigned char>& note_data, uint8_t channel);
	ZST_PLUGIN_EXPORT virtual void receive_cc(const std::vector<unsigned char>& note_data, uint8_t channel);
	ZST_PLUGIN_EXPORT virtual void receive_sysex(const std::vector<unsigned char>& sysex_data, uint8_t channel);

private:
	// Input plugs
	std::unique_ptr<showtime::ZstOutputPlug> m_received_note_on;
	std::unique_ptr<showtime::ZstOutputPlug> m_received_note_off;
	std::unique_ptr<showtime::ZstOutputPlug> m_received_cc;
	std::unique_ptr<showtime::ZstOutputPlug> m_received_sysex;
};
