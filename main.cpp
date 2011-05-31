#include "logger.h"
#include "unary_server.h"
#include "echo_client.h"
//#include "spec_echo_client.h"
//#include "stream_client.h"
//#include "null_client.h"
//#include "recv_client.h"

#ifdef WIN32
#pragma comment(lib, "ws2_32.lib")
#endif

int main(int argc, char* argv[])
{
	int port = 4097;
	if (argc > 1) {
		port = atoi(argv[1]);
		if (port < 1024 || port > 65535) {
			port = 10000;
		}
	}

#ifdef WIN32
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
#endif

	// Logger
	logger& trace = logger::instance();
	trace.config("log/", "Echo Server", "[%P] %t :", "CONSOLE,DEBUG");

	trace.info("Port:%d\n", port);
	
	// Start
	trace.info("The Echo Server started.");

	unary_server<listener<logger>, echo_client<logger>, lock, mutex, logger> svr;
	listener<logger> lsnr(&svr);
	lsnr.port(port);
	svr.set_listener(&lsnr);

	svr.start();
	svr.wait();

	trace.info("The Echo Server finished.");
	return 0;
}
