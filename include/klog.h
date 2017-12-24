/*
 * log.h
 *
 *  Created on: 2017/12/22
 *      Author: Think
 */

#ifndef INCLUDE_KLOG_H_
#define INCLUDE_KLOG_H_
#include <syslog.h>
#include <stdlib.h>

#define LOG_TAG "KPRINTER"
#ifdef DUMMY_PRINT
#define OPENLOG() (void*) 0
#define CLOSELOG() (void*) 0
#define LOGD(...) printf(LOG_TAG #__VA_ARGS__)
#else
#define OPENLOG() openlog(LOG_TAG, LOG_CONS | LOG_PID, 0)
#define LOGD( ...) syslog(LOG_DEBUG, __VA_ARGS__)
#define LOGE( ...) syslog(LOG_ERR, __VA_ARGS__)
#define LOGFATAL(...) do{ \
	syslog(LOG_ERR, __VA_ARGS__);\
	abort(); \
	} while(0)

#define CLOSELOG() closelog()
#endif

#endif /* INCLUDE_KLOG_H_ */
