#ifndef __UNARY_SERVER_H__
#define __UNARY_SERVER_H__

#include <vector>
#include <algorithm>

#include "server_i.h"
#include "listener.h"
#include "client.h"
#include "lock.h"
#include "mutex.h"
#include "null_logger.h"

#ifdef WIN32
#define SLEEP(s) Sleep(s*1000)
#endif
#ifdef Linux
#define SLEEP sleep
#endif

template<
		class Listener = listener<>,
		class Worker = client,
		template<class> class Lock = lock,
		class Mutex = mutex,
		class Logger = null_logger
>
class unary_server : public server_i, public thread {
public:
	unary_server() :
		m_listener(NULL), m_trace(Logger::instance()), m_stop(false) { }

	void set_listener(Listener* lsnr) { m_listener = lsnr; }
	
	virtual void start() { t_start(); }

	virtual void on_accepted(SOCKET sock, const struct sockaddr_in& addr) {
		m_trace.info("connected [%d]", sock);
		Worker* newWorker = new Worker(this, sock, addr);
		Lock<Mutex> guard(m_mtx);
		m_workers.push_back(newWorker);
		newWorker->t_start();
	}

	class isSame{ 
	public:
		isSame(SOCKET s) : m_s(s) { }
		bool operator()(const Worker* w) { return (w && w->sock() == m_s); }
	private:
		SOCKET m_s;
	};

	virtual void on_closed(SOCKET sock) {
		m_trace.info("socket %d closed.", sock);
		typename std::vector<Worker*>::iterator it = 
			std::find_if(m_workers.begin(), m_workers.end(), isSame(sock));
		if (it != m_workers.end()) {
			Lock<Mutex> guard(m_mtx);
			Worker* w = (*it);
			if (w) {
				delete w;
			}
		}
		m_workers.clear();
	}

	virtual void stop() { m_stop = true; }

protected:
	virtual void main() {
		m_listener->start();
		while ( !m_stop ) {
			SLEEP(1);
		}
	}

	Listener*				m_listener;
	std::vector<Worker*>	m_workers;
	Mutex					m_mtx;
	Logger&					m_trace;
	bool					m_stop;

};

#endif // __UNARY_SERVER_H__
