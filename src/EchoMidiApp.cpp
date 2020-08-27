#include <showtime/ShowtimeClient.h>
#include <showtime/ShowtimeServer.h>
#include <signal.h>

using namespace showtime;

int s_interrupted = 0;
typedef void (*SignalHandlerPointer)(int);

void s_signal_handler(int signal_value) {
	switch (signal_value) {
	case SIGINT:
		s_interrupted = 1;
	case SIGTERM:
		s_interrupted = 1;
	case SIGABRT:
		s_interrupted = 1;
	case SIGSEGV:
		s_interrupted = 1;
		signal(signal_value, SIG_DFL);
		raise(SIGABRT);
	default:
		break;
	}
}

void s_catch_signals() {
#ifdef WIN32
	SignalHandlerPointer previousHandler;
	previousHandler = signal(SIGSEGV, &s_signal_handler);
#else
	struct sigaction action;
	action.sa_handler = s_signal_handler;
	action.sa_flags = 0;
	sigemptyset(&action.sa_mask);
	sigaction(SIGINT, &action, NULL);
	sigaction(SIGTERM, &action, NULL);
	sigaction(SIGSEGV, &action, NULL);
#endif
}

class EchoMidi {
public:
	EchoMidi() {		
		m_server.init("EchoMidiServer");
		m_client.init("EchoMidi", true);
		m_client.auto_join_by_name("EchoMidiServer");

		ZstURI midi_factory_path = m_client.get_root()->URI() + ZstURI("midi_ports");
		auto midi_factory = dynamic_cast<ZstEntityFactory*>(m_client.find_entity(midi_factory_path));
		if (midi_factory) {
			ZstURIBundle bundle;
			midi_factory->get_creatables(&bundle);
			for (auto creatable : bundle) {
				m_client.create_entity(creatable, (std::string(creatable.last().path()) + "_echo").c_str());
			}
		}
	}

	~EchoMidi() {
		m_server.destroy();
		m_client.destroy();
	}

	ShowtimeClient& get_client() {
		return m_client;
	}


	ShowtimeServer& get_server() {
		return m_server;
	}

private:
	ShowtimeClient m_client;
	ShowtimeServer m_server;
};

int main(int argc, char** argv){
	s_catch_signals();

	EchoMidi echo;

	Log::app(Log::Level::notification, "Listening for midi events...");
	while (!s_interrupted)
		echo.get_client().poll_once();

	Log::app(Log::Level::notification, "Quitting");
	echo.get_client().destroy();
	echo.get_server().destroy();
	return 0;
}
