#ifndef __STREAM_CLIENT_H__
#define __STREAM_CLIENT_H__

#include "client.h"
#include "null_logger.h"

#include <string>

#define MESSAGE "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz\r\n"

template<class Logger = null_logger>
class stream_client : public client {
public:
	stream_client(server_i* serv, SOCKET sock, const struct sockaddr_in& addr) :
		client(serv, sock, addr), m_trace(Logger::instance()), 
		m_worker(sock) {
	}	

	virtual ~stream_client() {
		m_worker.stop();
	}

	void on_connected() {
		m_trace.debug("On connected !!!!!\n");
		m_worker.t_start();
	}

	void on_closed(int n) {
		m_trace.debug("On Closed...\n");
		m_worker.stop();
		m_worker.wait();
		m_trace.debug("The worker stopped.\n");
	}

	void on_read(const char* buffer, int nread) {
		m_trace.debug("[%d] read %d byte(s) ... ignored. ", m_sock, nread);
	}

	class stream_worker : public thread {
	public:
		stream_worker(SOCKET sock) :
		m_sock(sock), m_stop(false),
		m_message(MESSAGE), m_trace(Logger::instance()) {  }

		void stop() { m_stop = true; }

	protected:
		virtual void main() {
			m_trace.debug("started...:sock=%d\n", m_sock);
			while (!m_stop) {
				int n = send(m_sock, m_message.data(), m_message.size(), 0);
				m_trace.debug("sent %d byte(s)\n");
				usleep(100000);
				m_message = m_message.substr(1) + m_message.substr(0, 1);
			}
		}
		std::string m_message;
		SOCKET m_sock;
		bool m_stop;
		Logger& m_trace;
	};

protected:
	Logger&	m_trace;
	stream_worker m_worker;
};

#endif //  __STREAM_CLIENT_H__
