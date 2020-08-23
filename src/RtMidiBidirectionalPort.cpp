#include "RtMidiBidirectionalPort.h"

void RtMidiBidirectionalPort::send_midi(uint8_t* midibytes, size_t length)
{
	RtMidiOutPort::send_midi(midibytes, length);
}
