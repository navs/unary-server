#ifndef __SREVER_I_H__
#define __SREVER_I_H__

//
// Interface for Network Server
//
//		author: KIM Hyuntak
//		date: 2008-12-31
//

//#include <sys/types.h>
#include "default.h"

class server_i {
public:
	virtual void start() = 0;

	virtual void on_accepted(SOCKET sock, const struct sockaddr_in& addr) = 0;
	virtual void on_closed(SOCKET sock) = 0;
	virtual void stop() = 0;
};

#endif // __SREVER_I_H__
