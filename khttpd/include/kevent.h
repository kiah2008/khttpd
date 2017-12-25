/*
 * event.h
 *
 *  Created on: Dec 24, 2017
 *      Author: thinknow
 */

#ifndef EVENT_H_
#define EVENT_H_

#include <log.h>
typedef enum {
	SOCKET_STAT, SOCKET_DATA, SOCKET_MAX
} SOCKET_TYPE;

typedef struct SocketListenParam {
	SOCKET_TYPE soc_type;
	int fdListen;
	int port;
	void* (*eventHandler)(void*);
    u_short clientPort;
    int32_t clientAddr;
} SocketListenParam;

#define SOCKETID2NAME(id) (id== SOCKET_STAT?SOCK_STAT_NAME:SOCK_DATA_NAME)
#define SOCK_STAT_NAME "SOCKET_STAT"
#define SOCK_DATA_NAME "SOCKET_DATA"

int event_init(void* (*stat_handler)(void*), void*(*data_handler)(void*));


#endif /* EVENT_H_ */
