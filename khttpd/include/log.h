/*
 * log.h
 *
 *  Created on: 2017Äê12ÔÂ22ÈÕ
 *      Author: Think
 */

#ifndef INCLUDE_LOG_H_
#define INCLUDE_LOG_H_

#define LOGD(fortmat, ...) printf("[%s]" #fortmat,LOG_TAG, __VA_ARGS__)

#endif /* INCLUDE_LOG_H_ */
