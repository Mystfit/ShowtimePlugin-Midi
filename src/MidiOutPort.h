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


protected:
	ZST_PLUGIN_EXPORT virtual void send_note_off(unsigned char note, unsigned char velocity, unsigned char channel = 0);
	ZST_PLUGIN_EXPORT virtual void send_note_on(unsigned char note, unsigned char velocity, unsigned char channel = 0);
	ZST_PLUGIN_EXPORT virtual void send_poly_aftertouch(unsigned char note, unsigned char velocity, unsigned char channel = 0);
	ZST_PLUGIN_EXPORT virtual void send_cc(unsigned char controller, unsigned char value, unsigned char channel = 0);
	ZST_PLUGIN_EXPORT virtual void send_program_change(unsigned char program, unsigned char channel = 0);
	ZST_PLUGIN_EXPORT virtual void send_channel_aftertouch(unsigned char pressure, unsigned char channel = 0);
	ZST_PLUGIN_EXPORT virtual void send_pitch_bend(unsigned short value, unsigned char channel = 0);

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
