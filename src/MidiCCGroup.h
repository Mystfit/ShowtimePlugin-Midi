#pragma once

#include <showtime/entities/ZstComponent.h>
#include <showtime/entities/ZstPlug.h>
#include <memory>

#define MIDI_CC_COMPONENT_TYPE "midiccgroup"

class MidiCCGroup : public showtime::ZstComponent {
public:
	MidiCCGroup(const std::string& name, unsigned int cc);

	virtual void on_registered() override;
	virtual void compute(showtime::ZstInputPlug* plug) override;

private:
	unsigned int m_cc_plug;
	std::unique_ptr<showtime::ZstInputPlug> m_input_plug;
	std::unique_ptr<showtime::ZstOutputPlug> m_output_plug;
};
