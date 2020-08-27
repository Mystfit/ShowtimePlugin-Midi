#pragma once

#include <showtime/ZstExports.h>
#include <showtime/entities/ZstComponent.h>
#include <showtime/entities/ZstPlug.h>
#include <stdint.h>

#define MIDIOUT_COMPONENT_TYPE "midioutport"

class MidiOutPort : 
	public virtual showtime::ZstComponent
{
public:
	ZST_PLUGIN_EXPORT MidiOutPort(unsigned int port_number, const std::string& port_name);
	ZST_PLUGIN_EXPORT virtual void on_registered() override;
	ZST_PLUGIN_EXPORT virtual void compute(showtime::ZstInputPlug* plug) override;

	ZST_PLUGIN_EXPORT virtual void send_note_off(uint8_t note, uint8_t velocity, uint8_t channel = 0);
	ZST_PLUGIN_EXPORT virtual void send_note_on(uint8_t note, uint8_t velocity, uint8_t channel = 0);
	ZST_PLUGIN_EXPORT virtual void send_poly_aftertouch(uint8_t note, uint8_t velocity, uint8_t channel = 0);
	ZST_PLUGIN_EXPORT virtual void send_cc(uint8_t controller, uint8_t value, uint8_t channel = 0);
	ZST_PLUGIN_EXPORT virtual void send_program_change(uint8_t program, uint8_t channel = 0);
	ZST_PLUGIN_EXPORT virtual void send_channel_aftertouch(uint8_t pressure, uint8_t channel = 0);
	ZST_PLUGIN_EXPORT virtual void send_pitch_bend(uint16_t value, uint8_t channel = 0);

	ZST_PLUGIN_EXPORT virtual void send_midi(uint8_t* midibytes, size_t length) = 0;

private:
	// Output plugs
	std::unique_ptr<showtime::ZstInputPlug> m_note_on_plug;
	std::unique_ptr<showtime::ZstInputPlug> m_note_off_plug;
	std::unique_ptr<showtime::ZstInputPlug> m_polyaftertouch_plug;
	std::unique_ptr<showtime::ZstInputPlug> m_cc_plug;
	std::unique_ptr<showtime::ZstInputPlug> m_progchange_plug;
	std::unique_ptr<showtime::ZstInputPlug> m_channelaftertouch_plug;
	std::unique_ptr<showtime::ZstInputPlug> m_pitchbend_plug;
};
