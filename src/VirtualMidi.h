#pragma once

#include <teVirtualMIDI.h>
#include <showtime/entities/ZstComponent.h>
#include "MidiBidirectionalPort.h"

#define VIRTUALMIDI_COMPONENT_TYPE "virtualmidi"
#define MAX_SYSEX_BUFFER	65535

class VirtualMidi : 
	public virtual MidiBidirectionalPort
{
public:
	VirtualMidi(const std::string& name);

protected:
	void send_midi(uint8_t* midibytes, size_t length) override;

private:
	std::unique_ptr<LPVM_MIDI_PORT> m_virtualport;
};

void CALLBACK teVMCallback(LPVM_MIDI_PORT midiPort, LPBYTE midiDataBytes, DWORD length, DWORD_PTR dwCallbackInstance);
