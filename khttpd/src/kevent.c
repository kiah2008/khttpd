/*
 * event.c
 *
 *  Created on: Dec 24, 2017
 *      Author: thinknow
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <kevent.h>
#include <string.h>

#include <log.h>

#define KPORT1 8124
#define KPORT2 8125

static SocketListenParam socketStatParam = { SOCKET_STAT, -1, KPORT1,
		NULL, 0, 0 };
static pthread_mutex_t s_stat_param_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t s_stat_thread_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t s_stat_cond = PTHREAD_COND_INITIALIZER;
static SocketListenParam socketDataParam = { SOCKET_DATA, -1, KPORT2,
		NULL, 0, 0 };
static pthread_mutex_t s_data_param_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t s_data_thread_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t s_data_cond = PTHREAD_COND_INITIALIZER;
static SocketListenParam* socketListParams[SOCKET_MAX] = { &socketStatParam,
		&socketDataParam };

static void* startEventLoop(void* param);
static void* event_handler(void* param);

int event_init(void* (*stat_handler)(void*), void*(*data_handler)(void*)) {
	int i = 0;
	socketListParams[SOCKET_STAT]->eventHandler = stat_handler;
	startListen(socketListParams[SOCKET_STAT]);
	socketListParams[SOCKET_DATA]->eventHandler = data_handler;
	startListen(socketListParams[SOCKET_DATA]);
	return 0;
}

int createListenSocket(u_short *port) {
	int sockfd = 0;
	struct sockaddr_in name;
	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		error_die("socket");

	int reuse = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse))
			< 0) {
		error_die("setsockopet");
	}

	memset(&name, 0, sizeof(name));
	name.sin_family = AF_INET;
	name.sin_port = htons(*port);
	name.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(sockfd, (struct sockaddr *) &name, sizeof(name)) < 0)
		error_die("bind");
	if (*port == 0) {
		int namelen = sizeof(name);
		if (getsockname(sockfd, (struct sockaddr *) &name,
				(socklen_t *) &namelen) == -1)
			error_die("getsockname");
		*port = ntohs(name.sin_port);
	}

	if (listen(sockfd, 1) < 0)
		error_die("listen");
	return sockfd;
}

void startListen(SocketListenParam* listParam) {
	u_short port = listParam->port;
	int listenfd = -1;
	pthread_mutex_t* event_mutex;
	pthread_t eventThread;

	if (listParam == NULL) {
		LOGFATAL("startListen with null params");
	}
	if (listParam->soc_type == SOCKET_STAT) {
		event_mutex = &s_stat_param_mutex;
	} else {
		event_mutex = &s_data_param_mutex;
	}
	listenfd = createListenSocket(&port);
	LOGD(
			"startListen[%s] port %d, fd %d", SOCKETID2NAME(listParam->soc_type), port, listenfd);
	if (listenfd == -1) {
		LOGFATAL("failed to listen %d", port);
	}
	pthread_mutex_lock(event_mutex);
	listParam->fdListen = listenfd;
	listParam->port = port;
	pthread_mutex_unlock(event_mutex);

	if (pthread_create(&eventThread, NULL, startEventLoop, (void*) listParam)
			!= 0) {
		LOGFATAL("pthread_create");
	}
	LOGD( "startListen[%s] done!", SOCKETID2NAME(listParam->soc_type));
}

void* startEventLoop(void* param) {
	SocketListenParam* socketParam = (SocketListenParam*) param;
	int connfd_sock = -1;
	struct sockaddr_in client;
	int client_len = sizeof(client);
	char client_ip[16] = { 0 };
	pthread_attr_t attr;
	pthread_mutex_t* socket_mutex =
			socketParam->soc_type == SOCKET_STAT ?
					&s_stat_param_mutex : &s_data_param_mutex;

	pthread_mutex_t* thread_mutex =
			socketParam->soc_type == SOCKET_STAT ?
					&s_stat_thread_mutex : &s_data_thread_mutex;

	pthread_mutex_lock(thread_mutex);
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_t handlerThread;
	LOGD("startEventLoop %s", SOCKETID2NAME(socketParam->soc_type));
	while (1) {
		//loop waiting for client connection
		connfd_sock = accept(socketParam->fdListen, (struct sockaddr *) &client,
				(socklen_t *) &client_len);
		if (connfd_sock == -1) {
			LOGFATAL("accept");
		}
		strcpy(client_ip, inet_ntoa(client.sin_addr));
		LOGD(
				"[%s]recv client %s:%d \n", SOCKETID2NAME(socketParam->soc_type), client_ip, ntohs(client.sin_port));
		pthread_mutex_lock(socket_mutex);
		socketParam->clientPort = ntohs(client.sin_port);
		socketParam->clientAddr = client.sin_addr.s_addr;
		pthread_mutex_unlock(socket_mutex);
		if (pthread_create(&handlerThread, &attr, event_handler, param) != 0) {
			LOGFATAL("pthread_create");
		}
		pthread_cond_wait(&s_stat_cond, &s_stat_thread_mutex);
	}
	pthread_mutex_unlock(thread_mutex);
	close(socketParam->fdListen);
	LOGD(
			"eventLoop closed listen[%s] fd %d", SOCKETID2NAME(socketParam->soc_type), socketParam->fdListen);
	return NULL;
}

void* event_handler(void* param) {
	SocketListenParam* socketParam = (SocketListenParam*) param;
	pthread_cond_t* cond =
			socketParam->soc_type == SOCKET_STAT ? &s_stat_cond : &s_data_cond;
	pthread_mutex_t* socket_mutex =
			socketParam->soc_type == SOCKET_STAT ?
					&s_stat_param_mutex : &s_data_param_mutex;
	pthread_mutex_lock(socket_mutex);
	//TODO need lock here???
	struct in_addr sin_addr= {socketParam->clientAddr};
	if(socketParam->eventHandler != NULL) {
		socketParam->eventHandler(param);
	}
	LOGD(
			"[%s]event_handler closing %s:%d", SOCKETID2NAME(socketParam->soc_type), inet_ntoa(sin_addr), socketParam->clientPort);
	pthread_cond_signal(cond);
	pthread_mutex_unlock(socket_mutex);
	return NULL;
}
