#ifndef __NULL_CLIENT_H__
#define __NULL_CLIENT_H__

#include "client.h"
#include "null_logger.h"

template<class Logger = null_logger>
class null_client : public client {
public:
	null_client(server_i* serv, SOCKET sock, const struct sockaddr_in& addr) :
		client(serv, sock, addr), m_trace(Logger::instance()) { }

	void on_read(const char* buffer, int nread) {
		m_trace.debug("[%d] read %d byte(s)", m_sock, nread);
		// do nothing
	}

protected:
	Logger&	m_trace;
};

#endif //  __NULL_CLIENT_H__
