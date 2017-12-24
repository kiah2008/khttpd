#include <stdio.h>
#include <stdint.h>
#include <log.h>
#include <event.h>

void error_die(const char *sc) {
	LOGD("%s", sc);
	perror(sc);
	exit(1);
}

void* accept_request(void* param) {

	return NULL;
}

int main(int argc, char *argv[]) {
	OPENLOG();
	if(event_init()) {
		LOGE("event init failed!");
	}
	LOGD("starting sleep loop");
	while (1) {
		sleep(UINT32_MAX);
	}
	CLOSELOG();
	LOGD("exiting");
}
