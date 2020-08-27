#pragma once

#include "MidiCCGroup.h"

class Midi2DCC : public MidiCCGroup {
public:
	Midi2DCC(const std::string& name, unsigned int x_cc, unsigned int y_cc, showtime::ZstPlugDirection direction = showtime::ZstPlugDirection::NONE, MidiInPort* midi_in_port=NULL, MidiOutPort* midi_out_port=NULL);

	void compute(showtime::ZstInputPlug* plug) override;

private:
	int m_x_cc;
	int m_y_cc;
};
