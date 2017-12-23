#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <log.h>

#define KPORT 8088

void error_die(const char *sc) {
	LOGD("%s", sc);
	perror(sc);
	exit(1);
}

int startup(u_short *port) {
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

void* accept_request(void* param) {

}
;
int main(int argc, char *argv[]) {
	int listenfd = -1;
	u_short port = KPORT;
	int connfd_sock = -1;

	struct sockaddr_in client;
	int client_len = sizeof(client);
	char client_ip[16] = { 0 };

	pthread_t newthread;
	OPENLOG();
	listenfd = startup(&port);
	LOGD("rproxy running on port %d\n", port);

	while (1) {
		//loop waiting for client connection
		connfd_sock = accept(listenfd, (struct sockaddr *) &client,
				(socklen_t *) &client_len);
		if (connfd_sock == -1) {
			perror("accept");
			exit(1);
		}
		strcpy(client_ip, inet_ntoa(client.sin_addr));
		LOGD("recv client %s\n", client_ip);
		if (pthread_create(&newthread, NULL, accept_request,
				(void*) &connfd_sock) != 0) {
			perror("pthread_create");
			exit(1);
		}
	}
	close(listenfd);
	CLOSELOG();
	LOGD("exiting");
}
