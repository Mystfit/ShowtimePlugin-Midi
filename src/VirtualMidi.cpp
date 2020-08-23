#include "VirtualMidi.h"
#include "MidiConstants.h"
#include <showtime/ZstLogging.h>

using namespace showtime;

VirtualMidi::VirtualMidi(const std::string& name) :
	MidiBidirectionalPort(name, "", -1, "", -1), 
	ZstComponent(MIDI_BIDIRECTIONAL_COMPONENT_TYPE, name.c_str())
{
	auto port_name = std::string(name) + "_loopback";
	m_virtualport = std::make_unique<LPVM_MIDI_PORT>( virtualMIDICreatePortEx2(
		std::wstring(port_name.begin(), port_name.end()).c_str(), 
		teVMCallback, 
		(DWORD_PTR)this, 
		MAX_SYSEX_BUFFER, 
		TE_VM_FLAGS_PARSE_RX | TE_VM_FLAGS_PARSE_TX | TE_VM_FLAGS_INSTANTIATE_BOTH
	));

	if (!m_virtualport) {
		printf("could not create port: %d\n", GetLastError());
		return;
	}
}

void VirtualMidi::send_midi(uint8_t* midibytes, size_t length)
{
	if (!m_virtualport)
		return;
	
	if (!virtualMIDISendData(*m_virtualport.get(), midibytes, static_cast<DWORD>(length))) {
		Log::app(Log::Level::warn, "Virtual midi out: error sending data: {}" + GetLastError());
		return;
	}
}

void CALLBACK teVMCallback(LPVM_MIDI_PORT midiPort, LPBYTE midiDataBytes, DWORD length, DWORD_PTR dwCallbackInstance) {
	if ((NULL == midiDataBytes) || (0 == length)) {
		Log::app(Log::Level::warn, "VirtualMidi - Empty command; driver was probably shut down!");
		return;
	}
	Log::app(Log::Level::debug, "VirtualMidi port received midi message of length {}", length);
	((VirtualMidi*)dwCallbackInstance)->receive_midi(static_cast<uint8_t*>(midiDataBytes), length);
}
