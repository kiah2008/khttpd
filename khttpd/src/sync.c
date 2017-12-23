/*
 * sync.c
 *
 *  Created on: 2017年12月23日
 *      Author: Think
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <log.h>

void connect_server(const char* ip, int port) {
	int sockfd;
	int len;
	struct sockaddr_in address;
	int result;
	char ch = 'A';

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(ip);
	address.sin_port = htons(port);
	len = sizeof(address);
	result = connect(sockfd, (struct sockaddr *) &address, len);

	if (result == -1) {
		perror("connect failed");
		return;
	}
	write(sockfd, &ch, 1);
	read(sockfd, &ch, 1);
	LOGD("char from server = %c\n", ch);
	close(sockfd);
}
