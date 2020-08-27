#include "MidiBidirectionalPort.h"
#include "MidiCCGroup.h"
#include "Midi2DCC.h"
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
			parse_plug(child, this);
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
				else if (type == "plug") {
					parse_plug(child, component);
				}
			}
		}
	};
	
	// Save component in list to take ownership
	m_owned_entities.push_back(std::move(component));
	return m_owned_entities[m_owned_entities.size() - 1].get();
}

void MidiBidirectionalPort::parse_plug(const nlohmann::json& plug_data, ZstEntityBase* parent)
{
	if(!plug_data.contains("name") || !plug_data.contains("type"))
		return;

	auto type = plug_data["type"].get<std::string>();
	auto plug_type = (plug_data.contains("plug_type")) ? plug_data["plug_type"].get<std::string>() : "";
	auto direction = (plug_data.contains("direction")) ? plug_data["direction"].get<std::string>() : "both";
	
	ZstPlugDirection plug_dir(ZstPlugDirection::NONE);
	if (direction == "in")
		plug_dir = ZstPlugDirection::IN_JACK;
	else if (direction == "out")
		plug_dir = ZstPlugDirection::OUT_JACK;
	
	if (plug_type == "cc") {
		auto cc = plug_data["cc"].get<int>();
		auto plug = std::make_unique<MidiCCGroup>(plug_data["name"].get<std::string>().c_str(), cc, plug_dir, this, this);
		parent->add_child(plug.get());
		m_owned_entities.push_back(std::move(plug));
	}
	else if (plug_type == "2D_cc") {
		auto x_cc = (plug_data.contains("x_cc")) ? plug_data["x_cc"].get<int>() : -1;
		auto y_cc = (plug_data.contains("y_cc")) ? plug_data["y_cc"].get<int>() : -1;
		auto xygroup = std::make_unique<Midi2DCC>(plug_data["name"].get<std::string>().c_str(), x_cc, y_cc, plug_dir, this, this);
		parent->add_child(xygroup.get());
		m_owned_entities.push_back(std::move(xygroup));
	}
}
