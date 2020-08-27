#pragma once

#include "RtMidiInPort.h"
#include "RtMidiOutPort.h"
#include "MidiBidirectionalPort.h"

class RtMidiBidirectionalPort : 
	public virtual MidiBidirectionalPort,
	public RtMidiInPort,
	public RtMidiOutPort
{
public:
	ZST_PLUGIN_EXPORT RtMidiBidirectionalPort(const std::string& name, const std::string& in_port_name, int in_port_index, const std::string& out_port_name, int out_port_index, nlohmann::json map_data = nlohmann::json());
	void send_midi(uint8_t* midibytes, size_t length) override;
    
    void on_registered() override;
    void compute(showtime::ZstInputPlug* plug) override;
};
