#pragma once

#include <showtime/entities/ZstComponent.h>
#include <showtime/entities/ZstPlug.h>
#include <memory>
#include "MidiInPort.h"
#include "MidiOutPort.h"

#define MIDI_CC_COMPONENT_TYPE "midiccgroup"

class MidiCCGroup : public showtime::ZstComponent {
public:
	MidiCCGroup(const std::string& name, unsigned int cc, showtime::ZstPlugDirection direction = showtime::ZstPlugDirection::NONE, MidiInPort* midi_in_port=NULL, MidiOutPort* midi_out_port=NULL);

	virtual void on_registered() override;
	virtual void compute(showtime::ZstInputPlug* plug) override;

protected:
	MidiInPort* midi_in_port();
	MidiOutPort* midi_out_port();

private:
	int m_cc_plug;
	std::unique_ptr<showtime::ZstInputPlug> m_input_plug;
	std::unique_ptr<showtime::ZstOutputPlug> m_output_plug;
	MidiInPort* m_midi_in_port;
	MidiOutPort* m_midi_out_port;
};
