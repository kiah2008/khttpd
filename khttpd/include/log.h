/*
 * log.h
 *
 *  Created on: 2017��12��22��
 *      Author: Think
 */

#ifndef INCLUDE_LOG_H_
#define INCLUDE_LOG_H_

#define LOGD(fortmat, ...) printf("[%s]" #fortmat,LOG_TAG, __VA_ARGS__)

#endif /* INCLUDE_LOG_H_ */
