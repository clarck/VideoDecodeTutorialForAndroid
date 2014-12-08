LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

FFMPEG_PATH := ../ffmpeg
LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(FFMPEG_PATH)/include

LOCAL_MODULE    := ffmpeg_player
LOCAL_SRC_FILES += ffmpeg_jni.c 
LOCAL_SRC_FILES += ffmpeg.c

LOCAL_SHARED_LIBRARIES := ffmpeg
LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)