/*
 * log.h
 *
 *  Created on: 2017Äê12ÔÂ22ÈÕ
 *      Author: Think
 */

#ifndef INCLUDE_LOG_H_
#define INCLUDE_LOG_H_
#include <syslog.h>

#ifdef DUMMY_PRINT
#define OPENLOG() (void*) 0
#define CLOSELOG() (void*) 0
#define LOGD(fortmat, ...) printf("[%s]" #fortmat,LOG_TAG, __VA_ARGS__)
#else
#define OPENLOG() openlog(LOG_TAG, LOG_CONS | LOG_PID, 0)
#define LOGD(fortmat, ...) syslog(LOG_DEBUG, fortmat, __VA_ARGS__)
#define CLOSELOG() closelog()
#endif

#endif /* INCLUDE_LOG_H_ */
