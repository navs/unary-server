#ifndef	__NTHREAD_H__
#define	__NTHREAD_H__

//		author:	KIM Hyuntak
//		date:	2009-01-08

#ifdef Linux

#include <pthread.h>
typedef pthread_t tid_t
#define exit_thread	pthread_exit
#define join_thread(id,ret) pthread_join(id, ret)
#define create_thread(id, f, p) pthread_create(&id, NULL, f, p)

#endif

#ifdef WIN32

#include <Windows.h>

typedef HANDLE tid_t;
#define exit_thread	ExitThread
#define join_thread(id, ret) ret = WaitForSingleObject(id, INFINITE)
#define create_thread(id, f, p) id = CreateThread(NULL, 0, f, (LPVOID)p, 0, NULL)

#endif

class thread {
public:
	thread() { }
	virtual ~thread() { }

	int wait() { int ret; join_thread(m_id, ret); return ret; } 
	//int t_start() {return pthread_create(&m_id,NULL,thread::TF,(void*)this);}
	void t_start() { create_thread(m_id, thread::TF, this); }

protected:

	virtual void main() { }
#ifdef Linux
	static void* TF(void* arg)
#endif
#ifdef WIN32
		static DWORD WINAPI TF(LPVOID arg)
#endif
	{
		thread* pNT = reinterpret_cast<thread*>(arg);
		if (pNT) {
			pNT->main();
		}
		exit_thread(0);
		return NULL;
	}

	tid_t m_id;
};

#endif //	__NTHREAD_H__
