/*
 * ffmpeg_jni.c
 *
 *  Created on: Sep 1, 2014
 *      Author: clarck
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <jni.h>

#include "../include/ffmpeg_logger.h"
#include "../include/ffmpeg.h"

// 指定要注册的类，对应完整的java类名
#define JNIREG_CLASS "com/clarck/android/ffmpeg/MainActivity"

JNIEXPORT void JNICALL native_setDataSource(JNIEnv *env, jclass classzz, jstring path) {
	char *filepath = ffmpeg_jstringTostr(env, path);
	ffmpeg_setDataSource(filepath);
}

//Java和JNI函数的绑定
static JNINativeMethod method_table[] = {
	{ "setDataSource", "(Ljava/lang/String;)V", native_setDataSource }
};

//注冊native方法到java中
static int registerNativeMethods(JNIEnv *env, const char *className,
		JNINativeMethod *gMethods, int numMethods) {
	jclass clazz;
	clazz = (*env)->FindClass(env, className);
	if (clazz == NULL) {
		return JNI_FALSE;
	}

	if ((*env)->RegisterNatives(env, clazz, gMethods, numMethods) < 0) {
		return JNI_FALSE;
	}

	return JNI_TRUE;
}

//調用註冊方法
int register_ndk_load(JNIEnv *env) {
	return registerNativeMethods(env, JNIREG_CLASS, method_table,
			(int) (sizeof(method_table) / sizeof(method_table[0])));
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
	JNIEnv *env = NULL;
	jint result = -1;

	if ((*vm)->GetEnv(vm, (void**)&env, JNI_VERSION_1_6) != JNI_OK) {
		return result;
	}

	register_ndk_load(env);

	//返回JNI的版本
	return JNI_VERSION_1_6;
}

