#include "MidiBidirectionalPort.h"
#include "MidiCCGroup.h"
#include <showtime/ZstLogging.h>

using namespace showtime;

MidiBidirectionalPort::MidiBidirectionalPort(const std::string& name, const std::string& in_port_name, int in_port_index, const std::string& out_port_name, int out_port_index, nlohmann::json map_data) : 
	ZstComponent(MIDI_BIDIRECTIONAL_COMPONENT_TYPE, name.c_str()),
	MidiInPort(in_port_index, in_port_name),
	MidiOutPort(out_port_index, out_port_name),
	m_map_data(map_data)
{
}

void MidiBidirectionalPort::on_registered()
{
	// Register base midi plugs
	MidiInPort::on_registered();
	MidiOutPort::on_registered();

	parse_midi_map(m_map_data);
}

void MidiBidirectionalPort::compute(showtime::ZstInputPlug* plug)
{
	MidiOutPort::compute(plug);
}

void MidiBidirectionalPort::send_midi(uint8_t* midibytes, size_t length)
{
}

void MidiBidirectionalPort::parse_midi_map(const nlohmann::json& map_data)
{
	if (!map_data.contains("children"))
		return;

	for (auto child : map_data["children"]) {
		auto type = (child.contains("type")) ? child["type"].get<std::string>() : "";
		if (type == "component") {
			add_child(parse_component(child));
		}
		else if (type == "plug") {
			add_child(parse_plug(child));
		}
	}
}

ZstEntityBase* MidiBidirectionalPort::parse_component(const nlohmann::json& component_data)
{
	if (!component_data.contains("name"))
		return NULL;

	// Create component to hold our parsed components/plugs
	auto component = std::make_unique<ZstComponent>(component_data["name"].get<std::string>().c_str());
	component->entity_events()->entity_registered() += [this, component_data, component = component.get()](ZstEntityBase* entity) {
		// Create all children
		if (component_data.contains("children")) {
			for (auto child : component_data["children"]) {
				auto type = (child.contains("type")) ? child["type"].get<std::string>() : "";
				if (type == "component") {
					component->add_child(parse_component(child));
				}
				else if (type == "cc_plug") {
					component->add_child(parse_plug(child));
				}
			}
		}
	};
	
	// Save component in list to take ownership
	m_owned_entities.push_back(std::move(component));
	return m_owned_entities[m_owned_entities.size() - 1].get();
}

ZstEntityBase* MidiBidirectionalPort::parse_plug(const nlohmann::json& plug_data)
{
	if(!plug_data.contains("name") || !plug_data.contains("type"))
		return NULL;

	auto type = plug_data["type"].get<std::string>();
	auto cc = plug_data["cc"].get<int>();
	m_owned_entities.push_back(std::make_unique<MidiCCGroup>(plug_data["name"].get<std::string>().c_str(), cc));
	
	return m_owned_entities[m_owned_entities.size() - 1].get();
}
