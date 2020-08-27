#pragma once

#include <showtime/ZstExports.h>
#include <showtime/entities/ZstEntityFactory.h>
#include <showtime/ZstURI.h>
#include <showtime/ZstFilesystemUtils.h>
#include <nlohmann/json.hpp>


// Forwards

class RtMidi;
class RtMidiIn;
class RtMidiOut;


class ZST_CLASS_EXPORTED MidiFactory : public showtime::ZstEntityFactory
{
public:
	MidiFactory(const char* name);
	void parse_midi_port_maps(const fs::path& maps_dir);
private:
	/*	template<typename T>
	void register_midi_creatable(const std::string& port_name, int port_index, const nlohmann::json& map_data = nlohmann::json());*/
	void register_midi_port_map(nlohmann::json mapdata);
	int get_port_index(RtMidi* midi, const std::string& port_name);
#ifdef HAS_TEVIRTUALMIDI
	void register_virtual_midi_creatables();
#endif

	std::shared_ptr<RtMidiIn> m_midi_in;
	std::shared_ptr<RtMidiOut> m_midi_out;
};
