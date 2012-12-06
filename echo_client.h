#ifndef __ECHO_CLIENT_H__
#define __ECHO_CLIENT_H__

#include "client.h"
#include "null_logger.h"

template<class Logger = null_logger>
class echo_client : public client {
public:
	echo_client(server_i* serv, SOCKET sock, const struct sockaddr_in& addr) :
		client(serv, sock, addr), m_trace(Logger::instance()) {
		socklen_t namelen = sizeof(m_peer);
		getpeername(m_sock, (sockaddr*)&m_peer, &namelen);

		unsigned int hostIp = ntohl(m_peer.sin_addr.s_addr);

		m_trace.info("[%d] connected from %d.%d.%d.%d:%d",
			m_sock,
			hostIp >> 24 & 0xff,
			hostIp >> 16 & 0xff,
			hostIp >> 8 & 0xff,
			hostIp & 0xff,
			ntohs(m_peer.sin_port));

	}

	void on_read(const char* buffer, int nread) {
		m_trace.debug("[%d] read %d byte(s)", m_sock, nread);
		int nsent = 0;
		int ntotal = 0;
		while (ntotal < nread) {
			nsent = send(m_sock, buffer+ntotal, nread-ntotal, 0);
			if (nsent <= 0) {
				break;
			}
			ntotal += nsent;
		}
		m_trace.debug("[%d] sent %d of %d byte(s)", m_sock, ntotal, nread);
	}

	void on_closed(int ret)
	{
		unsigned int hostIp = ntohl(m_peer.sin_addr.s_addr);

		m_trace.info("[%d] closed from %d.%d.%d.%d:%d",
			m_sock,
			hostIp >> 24 & 0xff,
			hostIp >> 16 & 0xff,
			hostIp >> 8 & 0xff,
			hostIp & 0xff,
			ntohs(m_peer.sin_port));
	}

protected:
	Logger&	m_trace;

	struct sockaddr_in m_peer;
};

#endif //  __ECHO_CLIENT_H__
