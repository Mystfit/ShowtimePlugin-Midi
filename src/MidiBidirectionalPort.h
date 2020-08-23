#pragma once

#include "MidiInPort.h"
#include "MidiOutPort.h"
#include <showtime/entities/ZstComponent.h>
#include <nlohmann/json.hpp>
#include <vector>

#define FORCE_MIDI_MAP_LOAD 1
#define MIDI_BIDIRECTIONAL_COMPONENT_TYPE "midibidirectionalport"

class MidiBidirectionalPort : 
	public MidiInPort, 
	public MidiOutPort 
{
public:
	ZST_PLUGIN_EXPORT MidiBidirectionalPort(const std::string& name, const std::string& in_port_name, int in_port_index, const std::string& out_port_name, int out_port_index, nlohmann::json map_data=nlohmann::json());
	ZST_PLUGIN_EXPORT virtual void on_registered() override;
	ZST_PLUGIN_EXPORT virtual void compute(showtime::ZstInputPlug* plug) override;

	ZST_PLUGIN_EXPORT virtual void send_midi(uint8_t* midibytes, size_t length) override;


private:
	void parse_midi_map(const nlohmann::json& map_data);
	ZstEntityBase* parse_component(const nlohmann::json& component_data);
	ZstEntityBase* parse_plug(const nlohmann::json& plug_data);
	nlohmann::json m_map_data;
	
	std::vector< std::unique_ptr<showtime::ZstEntityBase> > m_owned_entities;
};
