#ifndef __RECV_CLIENT_H__
#define __RECV_CLIENT_H__

#include "client.h"
#include "null_logger.h"

#include <string>

template<class Logger = null_logger>
class recv_client : public client {
public:
	recv_client(server_i* serv, SOCKET sock, const struct sockaddr_in& addr) :
		client(serv, sock, addr), m_trace(Logger::instance()) { }

	void on_read(const char* buffer, int nread) {
		m_trace.debug("[%d] read %d byte(s)", m_sock, nread);

		std::string tmp(buffer, nread);
		m_trace.info("[%d] DATA:%s", m_sock, tmp.c_str());


		int nsent = 0;
		int ntotal = 0;
	}

protected:
	Logger&	m_trace;
};

#endif //  __RECV_CLIENT_H__
