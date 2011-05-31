#ifndef __LISTENER_H__
#define __LISTENER_H__

#include "default.h"

#ifdef Linux
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#endif
#include <cstring>

#include "thread.h"
#include "server_i.h"
#include "null_logger.h"

//
// Listener
//
//		author: KIM Hyuntak (navsis@gmail.com)
//		date: 2008-12-31
//
//		modified. 2009-01-08 navs
//			More generalized
//	

template<class Logger = null_logger>
class listener : public thread {
public:
	listener(server_i* serv) : m_serv(serv), m_trace(logger::instance()) { }

	void start() { thread::t_start(); }

	void port(unsigned short int port)		{ m_port = port; }
	unsigned short port() const				{ return m_port; }
	const SOCKET sock() const				{ return m_sock; }
	const struct sockaddr_in& addr() const	{ return m_addr; }
	int last_error() const					{ return m_err; }

	static const int NUM_BACKLOGS = 5;
#ifndef WIN32
	static const int INVALID_SOCKET = -1;
#endif

protected:
	//
	//	Initialize a server socket ( bind and listen )
	//
	bool init() {
		m_err = 0;
		m_sock = socket(AF_INET, SOCK_STREAM, 0);
		if (INVALID_SOCKET == m_sock) {
			m_err = -1;
			return false;
		}
		std::memset(&m_addr, 0, sizeof(m_addr));
		m_addr.sin_family = AF_INET;
		m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		m_addr.sin_port = htons(m_port);

		int v = 1;
		if (setsockopt(m_sock,SOL_SOCKET,SO_REUSEADDR,(char*)&v,sizeof(v))<0) {
			m_err = -2;
			return false;
		}
		if (bind(m_sock, (struct sockaddr*)&m_addr, sizeof(m_addr)) != 0) {
			m_err = -3;
			return false;
		}
		if (listen(m_sock, NUM_BACKLOGS) != 0) {
			m_err = -4;
			return false;
		}
		return true;
	}

	//
	// Listener main
	//
	virtual void main() {
		if (!init()) {
			m_trace.error("Listener initializing failed. Error = %d", m_err);
			m_serv->stop();
			return;
		}
		m_trace.info("Listening OK. (port=%d)", m_port);
		do {
			struct sockaddr_in newaddr;
			socklen_t addrsize = sizeof(newaddr);
			SOCKET newsock = accept(m_sock, 
					reinterpret_cast<struct sockaddr*>(&newaddr), &addrsize);
			if (newsock == INVALID_SOCKET) {
				m_trace.fatal("Listener acception failed.");
				m_serv->stop();
				break;
			}
			m_serv->on_accepted(newsock, newaddr);
		} while (true);
		closesocket(m_sock);
		m_serv->stop();
		m_trace.info("Listener finished.\n");
	}

	server_i*			m_serv;
	SOCKET				m_sock;
	struct sockaddr_in	m_addr;
	unsigned short		m_port;
	int					m_err;
	Logger&				m_trace;
};
#endif // __LISTENER_H__
