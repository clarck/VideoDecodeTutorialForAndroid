/*
 * logger.h
 *
 *  Created on: Sep 10, 2014
 *      Author: clarck
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <android/log.h>

#define APPNAME "player_log"

#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, APPNAME, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG , APPNAME, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO , APPNAME, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN , APPNAME, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR , APPNAME, __VA_ARGS__)

#endif /* LOGGER_H_ */
