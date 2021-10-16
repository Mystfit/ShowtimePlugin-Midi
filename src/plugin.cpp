#include "plugin.h"
#include "MidiFactory.h"
#include <showtime/ZstFilesystemUtils.h>
#include <showtime/ZstLogging.h>

namespace showtime {
	RtMidiPlugin::RtMidiPlugin() : ZstPlugin()
	{
	}

	RtMidiPlugin::~RtMidiPlugin()
	{
	}

	std::shared_ptr<RtMidiPlugin> showtime::RtMidiPlugin::create()
	{
		return std::make_shared<RtMidiPlugin>();
	}

	void showtime::RtMidiPlugin::init(const char* plugin_data_path)
	{
		fs::path data_path = plugin_data_path;
		auto midi_factory = std::make_unique<MidiFactory>("midi_ports");

		if (!data_path.empty()) {
			auto midi_data_path = data_path;
			midi_data_path.append(PLUGIN_NAME).append(MIDI_MAP_DIR);
			if(fs::exists(midi_data_path))
				midi_factory->parse_midi_port_maps(midi_data_path);
		}
		else {
			Log::app(Log::Level::warn, "ShowtimeMidiPlugin: No plugin data path set. Can't load midi maps");
		}

		add_factory(std::unique_ptr<ZstEntityFactory>(std::move(midi_factory)));
	}

	const char* showtime::RtMidiPlugin::name()
	{
		return PLUGIN_NAME;
	}

	int showtime::RtMidiPlugin::version_major()
	{
		return PLUGIN_MAJOR_VER;
	}

	int showtime::RtMidiPlugin::version_minor()
	{
		return PLUGIN_MINOR_VER;
	}

	int showtime::RtMidiPlugin::version_patch()
	{
		return PLUGIN_PATCH_VER;
	}
}
