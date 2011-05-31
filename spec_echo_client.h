#ifndef __SPEC_ECHO_CLIENT_H__
#define __SPEC_ECHO_CLIENT_H__

#include "client.h"
#include "null_logger.h"

using std::string;

template<class Logger = null_logger>
class spec_echo_client : public client {
public:
	spec_echo_client(server_i* serv, SOCKET sock, const struct sockaddr_in& addr) :
		client(serv, sock, addr), m_trace(Logger::instance()), m_mode(mode_header)  { }

	void on_read(const char* buffer, int nread) {
		m_trace.debug("[%d] read %d byte(s)", m_sock, nread);

		m_buffer += string(buffer, nread);
		m_trace.info("[%d] the buffer size is %d", m_sock, m_buffer.size());
		if (m_mode == mode_header) {
			if (m_buffer.size() >= 4) {
				m_size = static_cast<unsigned char>(m_buffer[2]) * 256 +
						static_cast<unsigned char>(m_buffer[3]);
				m_trace.info("[%d] the size is %d", m_sock, m_size);
				if (m_size > 1500) {
					m_trace.error("[%d] the size is too much larger (%d)",
									m_sock, m_size);
					close(m_sock);
				}
				m_mode = mode_send;
			}
		}

		if (m_mode == mode_read) {
			m_trace.info("READ MODE");
			if (m_buffer.size() >= m_size + 4) {
				m_mode = mode_send;
			}
		}

		// send
		if (m_mode == mode_send) {
			// error
			sleep(12);
			/*
			close(m_sock);
			return;
			*/

			m_trace.info("SEND MODE");
			int nsent = 0;
			int ntotal = 0;
			int nsize = m_buffer.size();
			const char* p = m_buffer.data();
			while (ntotal < nsize) {
				nsent = send(m_sock, p+ntotal, nsize-ntotal, 0);
				if (nsent <= 0) {
					break;
				}
				ntotal += nsent;
			}
			m_trace.debug("[%d] sent %d of %d byte(s)", m_sock, ntotal, nread);
			close(m_sock);
		}
	}

protected:
	Logger&	m_trace;
	string m_buffer;
	int m_size;
	enum { mode_header, mode_read , mode_send } m_mode;
	static const int header_len = 4;
};

#endif //  __SPEC_ECHO_CLIENT_H__
