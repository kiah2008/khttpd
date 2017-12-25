#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stddef.h>

void usage() {
	puts("Usage:");
	puts("simpleclient -ip ip:port-str -d xml-data");
	exit(1);
}
int main(int argc, char *argv[]) {
	int sockfd;
	int len;
	struct sockaddr_in address;
	int result;
	int i = 1;
	char rcvdata[1024];
	char* ipstr = NULL, *datastr = NULL;
	uint16_t port = 0;
	if (argc < 3) {
		usage();
	}
	printf("argc %d\n", argc);
	while(i < argc ) {
		if(strcasecmp("-ip", argv[i]) == 0 && (i+1) < argc) {
			i++;
			ipstr = argv[i];
		} else if (strcasecmp("-d", argv[i]) == 0 && (i+1) < argc) {
			i++;
			datastr = argv[i];
		}
		i++;
	}

	i = 0;
	while (i< strlen(ipstr) - 1 && ipstr[i] != ':') {
		i++;
	}
	if (i >= strlen(ipstr)-1) {
		usage();
	}
	ipstr[i++] = '\0';
	port = atoi(ipstr+i);
	printf("connecting %s:%d!\ninfo:\n %s!\n", ipstr, port,
			(datastr == NULL ? "NULL" : datastr));
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(ipstr);
	address.sin_port = htons(port);
	len = sizeof(address);
	result = connect(sockfd, (struct sockaddr *) &address, len);

	if (result == -1) {
		perror("oops: client error!\n");
		exit(1);
	}
	printf("connected successfully!\n");
	if(strlen(datastr)>0)
		write(sockfd, datastr, strlen(datastr));
	i = read(sockfd, rcvdata, 1024);
	rcvdata[i] = '\0';
	printf("Rcv from server = %s\n", rcvdata);
	close(sockfd);
	exit(0);
}
