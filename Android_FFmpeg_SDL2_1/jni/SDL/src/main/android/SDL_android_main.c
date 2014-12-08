/*
    SDL_android_main.c, placed in the public domain by Sam Lantinga  3/13/14
*/
#include "../../SDL_internal.h"

#ifdef __ANDROID__

/* Include the SDL main definition header */
#include "SDL_main.h"

/*******************************************************************************
                 Functions called by JNI
*******************************************************************************/
#include <jni.h>

/* Called before SDL_main() to initialize JNI bindings in SDL library */
extern void SDL_Android_Init(JNIEnv* env, jclass cls);

char* jstringTostr(JNIEnv* env, jstring jstr) {
	char* pStr = NULL;

	jclass jstrObj = (*env)->FindClass(env, "java/lang/String");
	jstring encode = (*env)->NewStringUTF(env, "utf-8");
	jmethodID methodId = (*env)->GetMethodID(env, jstrObj, "getBytes",
			"(Ljava/lang/String;)[B");
	jbyteArray byteArray = (jbyteArray) (*env)->CallObjectMethod(env, jstr,
			methodId, encode);
	jsize strLen = (*env)->GetArrayLength(env, byteArray);
	jbyte *jBuf = (*env)->GetByteArrayElements(env, byteArray, JNI_FALSE);

	if (jBuf > 0) {
		pStr = (char*) malloc(strLen + 1);

		if (!pStr) {
			return NULL ;
		}

		memcpy(pStr, jBuf, strLen);

		pStr[strLen] = 0;
	}

	(*env)->ReleaseByteArrayElements(env, byteArray, jBuf, 0);

	return pStr;
}

/* Start up the SDL app */
void Java_org_libsdl_app_SDLActivity_nativeInit(JNIEnv* env, jclass cls, jobject obj)
{
    /* This interface could expand with ABI negotiation, calbacks, etc. */
    SDL_Android_Init(env, cls);

    SDL_SetMainReady();

    char *filePath = jstringTostr(env, obj);

    /* Run the application code! */
    int status;
    char *argv[2];
    argv[0] = SDL_strdup("SDL_app");
    argv[1] = filePath;
    status = SDL_main(1, argv);

    /* Do not issue an exit or the whole application will terminate instead of just the SDL thread */
    /* exit(status); */
}

#endif /* __ANDROID__ */

/* vi: set ts=4 sw=4 expandtab: */
