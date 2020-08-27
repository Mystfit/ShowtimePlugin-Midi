#include "MidiFactory.h"
#include <RtMidi.h>
#include <showtime/ZstLogging.h>
#include <fstream>
#include <memory>

#include "RtMidiInPort.h"
#include "RtMidiOutPort.h"
#include "RtMidiBidirectionalPort.h"

using namespace showtime;
using namespace nlohmann;

MidiFactory::MidiFactory(const char* name) : 
	showtime::ZstEntityFactory(name),
	m_midi_in(std::make_shared<RtMidiIn>()),
	m_midi_out(std::make_shared<RtMidiOut>())
{
	try {
		m_midi_in = std::make_shared<RtMidiIn>();
	}
	catch (RtMidiError& error) {
		Log::app(Log::Level::error, "Midi In construction failed: {}", error.getMessage().c_str());
	}
	
	try {
		m_midi_out = std::make_shared<RtMidiOut>();
	}
	catch (RtMidiError& error) {
		Log::app(Log::Level::error, "Midi Out construction failed: {}", error.getMessage().c_str());
	}

#ifdef HAS_TEVIRTUALMIDI
	register_virtual_midi_creatables();
#endif
}

void MidiFactory::parse_midi_port_maps(const fs::path& map_dir)
{
	for (auto mapping_file : fs::directory_iterator(map_dir)) {
		if (!fs::is_regular_file(mapping_file))
			continue;

		if (mapping_file.path().extension() != fs::path(".json"))
			continue;
        
		std::ifstream input_stream(mapping_file.path().string());
		json mapping_json;
		input_stream >> mapping_json;
		register_midi_port_map(mapping_json);
	}
}

void MidiFactory::register_midi_port_map(json map_data)
{
	// Verify json has name tag
	if (!map_data.contains("name"))
		return;

	std::string name = map_data["name"];

	// Make sure we can apply our map to one of the ports the map specifies
	int in_port_index = (map_data.contains("port_in_name")) ? get_port_index(m_midi_in.get(), map_data["port_in_name"].get<std::string>()) : -1;
	int out_port_index = (map_data.contains("port_out_name")) ? get_port_index(m_midi_out.get(), map_data["port_out_name"].get<std::string>()) : -1;
	auto in_port_name = (map_data.contains("port_in_name")) ? map_data["port_in_name"].get<std::string>() : "";
	auto out_port_name = (map_data.contains("port_out_name")) ? map_data["port_out_name"].get<std::string>() : "";

	if (in_port_index >= 0 && out_port_index >= 0 || FORCE_MIDI_MAP_LOAD) {
		ZstURI creatable_name(map_data["name"].get<std::string>().c_str());
		this->add_creatable<RtMidiBidirectionalPort>(creatable_name, [in_port_index, in_port_name, out_port_index, out_port_name, map_data](const char* e_name) -> std::unique_ptr<ZstEntityBase> {
			return std::make_unique<MidiBidirectionalPort>(std::string(e_name), in_port_name, in_port_index, out_port_name, out_port_index, map_data);
			});
		/*register_midi_creatable<MidiBidirectionalPort>(in_port_name, in_port_index, out_port_name, out_port_index, map_data);*/
	}
	else if (in_port_index >= 0) {
		//register_midi_creatable<MidiInPort>(in_port_name, in_port_index, map_data);
	}
	else if (out_port_index >= 0) {
		//register_midi_creatable<MidiOutPort>(out_port_name, out_port_index, map_data);
	}
	else {
		// Neither port matches our map
	}
}

int MidiFactory::get_port_index(RtMidi* midi, const std::string& port_name)
{
	auto num_ports = midi->getPortCount();
	for (size_t port_index = 0; port_index < num_ports; ++port_index) {
		if (port_name == midi->getPortName())
			return static_cast<int>(port_index);
	}
	return -1;
}

//template<typename T>
//void MidiFactory::register_midi_creatable(const std::string& port_name, int port_index, const nlohmann::json& map_data)
//{
//	this->add_creatable<T>(creatable_name, [port_index, port_name, map_data](const char* e_name) -> std::unique_ptr<ZstEntityBase> {
//		ZstURI creatable_name(port_name.c_str());
//		return std::make_unique<T>(creatable_name.path(), port_index, port_name.c_str(), map_data);
//		});
//}

#ifdef HAS_TEVIRTUALMIDI
#include "VirtualMidi.h"
void MidiFactory::register_virtual_midi_creatables() {
	this->add_creatable<VirtualMidi>("virtualMidi", [](const char* e_name) -> std::unique_ptr<ZstEntityBase> {
		return std::make_unique<VirtualMidi>(e_name);
	});
}
#endif
