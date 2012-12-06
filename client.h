//
//	Default client thread
//		KIM Hyuntak (navsis@gmail.com)
//		2008-12-31
//
//	- Make your worker class by subclassing 'client' class
//	- Implement your on_connected(), on_read(), on_closed() if needed.
//	- Must call client(...) in the constructor of the derived class.
//
#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "default.h"
#ifdef WIN32
#include <Winsock2.h>
typedef int socklen_t;
#endif
#if defined(Linux) || defined(Darwin)
#include <sys/socket.h>
#define closesocket close
#endif

#include "server_i.h"
#include "thread.h"

class client : public thread {
public:
	client(server_i* serv, SOCKET sock, const struct sockaddr_in& addr)
		: m_sock(sock), m_serv(serv), m_addr(addr) {}

	const SOCKET sock() const { return m_sock; }
	const struct sockaddr_in& addr() const { return m_addr; }
	const struct sockaddr_in& peer() const { return m_peer; }

	static const int DEFAULT_BUFFER_SIZE=4096;
protected:
	virtual void main() {
		socklen_t psize = sizeof(m_peer);
		getpeername(m_sock, (struct sockaddr*)&m_peer, &psize);

		on_connected();

		const int bufsize = DEFAULT_BUFFER_SIZE;
		char buf[bufsize];
		int nread;

		while(true) {
			nread = recv(m_sock, buf, bufsize, 0);
			if (nread <= 0) {
				break;
			}
			on_read(buf, nread);
		}
		//closesocket(m_sock);
		on_closed(nread);
		//m_serv->on_closed(m_sock);
	}
		// Impelement on the inherited class.
	virtual void on_connected() { }
	virtual void on_closed(int ret) { }
	virtual void on_read(const char* buffer, int nread) { }

	server_i* m_serv;
	SOCKET m_sock;
	struct sockaddr_in m_addr;
	struct sockaddr_in m_peer;
};
#endif // __CLIENT_H__
