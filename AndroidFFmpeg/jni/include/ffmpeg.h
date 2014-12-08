/*
 * ffmpeg.h
 *
 *  Created on: Sep 1, 2014
 *      Author: clarck
 */

#ifndef FFMPEG_H_
#define FFMPEG_H_

char* ffmpeg_jstringTostr(JNIEnv* env, jstring jstr);
void ffmpeg_setDataSource(char *path);

#endif /* FFMPEG_H_ */
