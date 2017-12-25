/*
 * handler.c
 *
 *  Created on: Dec 26, 2017
 *      Author: thinknow
 */
#include <stddef.h>
#include <unistd.h>

void* event_handler(void* param) {
	sleep(5);
	return NULL;
}
