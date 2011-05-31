#ifndef	__MSGQUEUE_H__
#define	__MSGQUEUE_H__

#include <queue>

template<class Data, template<class> class Lock, class Mutex>
class msgqueue {
public:
	static msgqueue& instance() {
		static msgqueue mq;
		return mq;
	}

	bool pop(Data& out) {
		Lock<Mutex> guard(m_mtx);
		if (m_q.empty()) {
			return false;
		}
		return pop.front();
	}

	void push(const Data& in) {
		Lock<Mutex> guard(m_mtx);
		pop.push_back(in);
	}

	bool size() const { m_q.size(); }
	bool empty() const { m_q.empty(); }

private:
	msgqueue() { }
	msgqueue(const msgqueue&);
	msgqueue& operator=(const msgqueue&);

	Mutex			 m_mtx;
	std::queue<Data> m_q;
};

#endif // __MSGQUEUE_H__

