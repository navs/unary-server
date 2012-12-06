#ifndef	__DEFAULT_H__
#define	__DEFAULT_H__

#if defined(Linux) || defined(Darwin)
typedef int SOCKET;
#endif

#ifdef WIN32
#include <Winsock2.h>
#endif

#endif // __DEFAULT_H__
