#ifndef __MUTEX_H__
#define __MUTEX_H__

#ifdef Linux

#include <pthread.h>

class mutex {
public:
	mutex() 		{ pthread_mutex_init(&m_mtx, NULL); }
	~mutex() 		{ pthread_mutex_destroy(&m_mtx); }
	void lock()		{ pthread_mutex_lock(&m_mtx); }
	void unlock() 	{ pthread_mutex_unlock(&m_mtx); }
protected:
	pthread_mutex_t	m_mtx;
};

#endif

#ifdef WIN32

#include <Windows.h>
class mutex {
public:
	mutex() { InitializeCriticalSection(&m_cs); }
	~mutex() { DeleteCriticalSection(&m_cs); }
	void lock()	{ EnterCriticalSection(&m_cs); }
	void unlock() { LeaveCriticalSection(&m_cs); }
private:
	CRITICAL_SECTION m_cs;
};

#endif

#endif // __MUTEX_H__
