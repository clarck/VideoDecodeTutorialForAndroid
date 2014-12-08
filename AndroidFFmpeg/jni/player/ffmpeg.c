/*
 * ffmpeg.c
 *
 *  Created on: Sep 1, 2014
 *      Author: clarck
 */
#include <jni.h>
#include <inttypes.h>
#include <android/native_window_jni.h>
#include "../include/ffmpeg.h"
#include "../include/ffmpeg_logger.h"
#include "../ffmpeg/include/libavcodec/avcodec.h"
#include "../ffmpeg/include/libavformat/avformat.h"
#include "../ffmpeg/include/libavutil/pixfmt.h"
#include "../ffmpeg/include/libswscale/swscale.h"

char* ffmpeg_jstringTostr(JNIEnv* env, jstring jstr) {
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

void ffmpeg_setDataSource(char *file_path) {
	LOGI("ffmpeg_setDataSource:%s", file_path);

	AVFormatContext *pFormatCtx;
	AVCodecContext *pCodecCtx;
	AVCodec *pCodec;
	AVFrame *pFrame, *pFrameYUV;
	AVPacket *packet;
	uint8_t *out_buffer;

	static struct SwsContext *img_convert_ctx;

	int videoStream, i, numBytes;
	int ret, got_picture;

	av_register_all();
	pFormatCtx = avformat_alloc_context();

	if (avformat_open_input(&pFormatCtx, file_path, NULL, NULL) != 0) {
		LOGE("can't open the file. \n");
		return;
	}

	if (avformat_find_stream_info(pFormatCtx, NULL) < 0) {
		LOGE("Could't find stream infomation.\n");
		return;
	}

	videoStream = 1;
	for (i = 0; i < pFormatCtx->nb_streams; i++) {
		if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
			videoStream = i;
		}
	}

	if (videoStream == -1) {
		LOGE("Didn't find a video stream.\n");
		return;
	}

	pCodecCtx = pFormatCtx->streams[videoStream]->codec;
	pCodec = avcodec_find_decoder(pCodecCtx->codec_id);

	if (pCodec == NULL) {
		LOGE("Codec not found.\n");
		return;
	}

	if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
		LOGE("Could not open codec.\n");
		return;
	}

	pFrame = av_frame_alloc();
	pFrameYUV = av_frame_alloc();

	numBytes = avpicture_get_size(AV_PIX_FMT_YUV420P, pCodecCtx->width,
			pCodecCtx->height);
	out_buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
	avpicture_fill((AVPicture *) pFrameYUV, out_buffer, AV_PIX_FMT_YUV420P,
			pCodecCtx->width, pCodecCtx->height);

	int y_size = pCodecCtx->width * pCodecCtx->height;

	packet = (AVPacket *) malloc(sizeof(AVPacket));
	av_new_packet(packet, y_size);

	av_dump_format(pFormatCtx, 0, file_path, 0);

	while (av_read_frame(pFormatCtx, packet) >= 0) {
		if (packet->stream_index == videoStream) {
			ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture,
					packet);
			LOGI("pFrame: pkt_pts:%ld, pict_type:%d", (long)pFrame->pkt_pts, pFrame->pict_type);

			if (ret < 0) {
				LOGE("decode error.\n");
				return;
			}

			if (got_picture) {
				//TODO 此处可以将解码出来的图片保存起来。
			}
		}
		av_free_packet(packet);
	}

	av_free(out_buffer);
	av_free(pFrameYUV);
	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);
}
