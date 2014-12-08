LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := ffmpeg
LOCAL_SRC_FILES := /data/kplayer/build/ffmpeg/output/libffmpeg.so
include $(PREBUILT_SHARED_LIBRARY)
