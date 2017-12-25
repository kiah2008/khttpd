#include <stdio.h>
#include <stdint.h>
#include <log.h>
#include <pevent.h>

void error_die(const char *sc) {
	LOGD("%s", sc);
	perror(sc);
	exit(1);
}

void* accept_request(void* param) {

	return NULL;
}

void* event_handler(void* param) {
	return NULL;
}
int main(int argc, char *argv[]) {
	OPENLOG();
	if(event_init(event_handler, event_handler)) {
		LOGE("event init failed!");
	}
	LOGD("starting sleep loop");
	while (1) {
		sleep(UINT32_MAX);
	}
	CLOSELOG();
	LOGD("exiting");
}
