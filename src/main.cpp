#include <signal.h>
#include <thread>
#include <chrono>

#include "upnp.hpp"
#include "server.hpp"

int main()
{
	uint16_t in_port = 33333;
    
	server server;
	server.start("0.0.0.0", in_port);

	upnp up;
	up.port_mapping(in_port, 43333);

	getchar();

	return 0;
}