#define BOOST_TEST_MODULE Showtime Midi Plugin

#include <showtime/ShowtimeClient.h>
#include <showtime/ShowtimeServer.h>
#include <showtime/ZstFilesystemUtils.h>
#include <boost/dll.hpp>
#include <boost/test/unit_test.hpp>
#include <memory>

using namespace showtime;


struct FixtureServer {
	FixtureServer() : 
		server(std::make_unique<ShowtimeServer>()),
		server_name(boost::unit_test::framework::current_test_case().full_name() + "_server")
	{
		server->init((boost::unit_test::framework::current_test_case().full_name() + "_server").c_str());
	}

	~FixtureServer() {
		server->destroy();
	}

	std::string server_name;

private:
	std::unique_ptr<ShowtimeServer> server;
};

struct FixtureClient {
	FixtureClient() :
		client(std::make_unique<ShowtimeClient>())
	{
		fs::path plugin_path = fs::path(boost::dll::program_location().string()).parent_path().append("plugins");
		client->set_plugin_path(plugin_path.string().c_str());
		client->set_plugin_data_path(plugin_path.append("plugin_data").string().c_str());
		client->init((boost::unit_test::framework::current_test_case().full_name() + "_client").c_str(), true);
	}

	~FixtureClient() {
		client->destroy();
	}

	std::unique_ptr<ShowtimeClient> client;
};

struct FixtureClientServer : public FixtureClient, public FixtureServer {
	FixtureClientServer() {
		client->auto_join_by_name(server_name.c_str());
	}
};


// ------------------------


BOOST_FIXTURE_TEST_CASE(load_plugin, FixtureClient) {
	auto plugins = client->plugins();
	auto it = std::find_if(plugins.begin(), plugins.end(), [](const std::shared_ptr<ZstPlugin>& plugin) {return strcmp(plugin->name(), "ShowtimePluginMidi") == 0; });
	bool found_plugin = (it != plugins.end());
	BOOST_TEST(found_plugin);
}

BOOST_FIXTURE_TEST_CASE(list_midi_creatables, FixtureClient) {
	ZstEntityFactoryBundle bundle;
	client->get_root()->get_factories(&bundle);
	auto factory_it = *std::find_if(bundle.begin(), bundle.end(), [](auto it) {
		return ZstURI("midi_ports") == it->URI().last();
	});
	ZstURIBundle creatables_bundle;
	factory_it->get_creatables(&creatables_bundle);

	for (auto path : creatables_bundle) {
		Log::app(Log::Level::debug, "Name: {}", path.path());
	}
}

BOOST_FIXTURE_TEST_CASE(minilouge_creatable, FixtureClient) {
	auto entity = client->create_entity(client->get_root()->URI() + ZstURI("midi_ports/minilouge"), "created_minilouge");
	BOOST_REQUIRE(entity);
}

BOOST_FIXTURE_TEST_CASE(virtualmidi_create, FixtureClientServer) {
	ZstURI virtualmidi_path = client->get_root()->URI() + ZstURI("midi_ports/virtualMidi");
	std::string virtualmidi_name = "test_virtual_midi";

	auto virtualmidi_component = client->create_entity(virtualmidi_path, virtualmidi_name.c_str());
	BOOST_REQUIRE(virtualmidi_component);
	BOOST_TEST(virtualmidi_component->URI().last() == ZstURI(virtualmidi_name.c_str()));
}

BOOST_FIXTURE_TEST_CASE(virtualmidi_receive, FixtureClientServer) {
	ZstURI virtualmidi_path = client->get_root()->URI() + ZstURI("midi_ports/virtualMidi");
	auto virtualmidi_component = client->create_entity(virtualmidi_path, "test_virtual_midi");
	
	ZstInputPlug* vmidi_note_send = dynamic_cast<ZstInputPlug*>(client->find_entity(virtualmidi_component->URI() + ZstURI("send_noteon")));
	ZstOutputPlug* vmidi_note_recv = dynamic_cast<ZstOutputPlug*>(client->find_entity(virtualmidi_component->URI() + ZstURI("recv_note")));
	BOOST_REQUIRE(vmidi_note_send);
	BOOST_REQUIRE(vmidi_note_recv);
	
	auto output_comp = std::make_unique<ZstComponent>("output");
	client->get_root()->add_child(output_comp.get());
	auto output_plug = std::make_unique<ZstOutputPlug>("out", ZstValueType::IntList);
	output_comp->add_child(output_plug.get());

	auto input_comp = std::make_unique<ZstComponent>("input");
	client->get_root()->add_child(input_comp.get());
	auto input_plug = std::make_unique<ZstInputPlug>("in", ZstValueType::IntList);
	input_comp->add_child(input_plug.get());

	client->connect_cable(vmidi_note_send, output_plug.get());
	client->connect_cable(input_plug.get(), vmidi_note_recv);
	
	output_plug->append_int(60);
	output_plug->append_int(60);
	output_plug->append_int(0);
	output_plug->fire();
	
	int max_loops = 200;
	while (input_plug->size() == 0 && --max_loops > 0) {
		client->poll_once();
	}
	BOOST_REQUIRE(input_plug->size() == 4);
	auto channel = input_plug->int_at(1);
	auto note = input_plug->int_at(2);
	auto velocity = input_plug->int_at(3);
	BOOST_TEST(channel == 0);
	BOOST_TEST(note == 60);
	BOOST_TEST(velocity == 60);
}