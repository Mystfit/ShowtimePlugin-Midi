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
	void send_midi(uint8_t* midibytes, size_t length) override;
    
    void on_registered() override;
    void compute(showtime::ZstInputPlug* plug) override;
};
