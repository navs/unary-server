#ifndef __LOCK_H__
#define __LOCK_H__

template<class Mutex>
class lock {
public:
	lock(Mutex& mtx) : m_mtx(mtx) { m_mtx.lock(); }
	~lock() { m_mtx.unlock(); }
protected:
	Mutex m_mtx;
};

#endif //  __LOCK_H__
