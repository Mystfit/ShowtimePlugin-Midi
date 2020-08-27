#include "RtMidiBidirectionalPort.h"

void RtMidiBidirectionalPort::send_midi(uint8_t* midibytes, size_t length)
{
	RtMidiOutPort::send_midi(midibytes, length);
}

void RtMidiBidirectionalPort::on_registered() { 
    RtMidiOutPort::on_registered();
    RtMidiInPort::on_registered();
}

void RtMidiBidirectionalPort::compute(showtime::ZstInputPlug* plug){
    RtMidiOutPort::compute(plug);
}

