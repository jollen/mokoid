/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//#define LOG_NDEBUG 0
#define LOG_TAG "FFmpegPlayer"
#include <utils/Log.h>

#include "FFmpegPlayer.h"

namespace android {

FFmpegPlayer::FFmpegPlayer() {
    ALOGV("FFmpegPlayer");
}

FFmpegPlayer::~FFmpegPlayer() {
    ALOGV("~FFmpegPlayer");
    reset();
}

status_t FFmpegPlayer::initCheck() {
    ALOGV("initCheck");
    return OK;
}

status_t FFmpegPlayer::setUID(uid_t uid) {
    return OK;
}

status_t FFmpegPlayer::setDataSource(
        const sp<IMediaHTTPService> &httpService,
        const char *url,
        const KeyedVector<String8, String8> *headers) {
    return OK;
}

// Warning: The filedescriptor passed into this method will only be valid until
// the method returns, if you want to keep it, dup it!
status_t FFmpegPlayer::setDataSource(int fd, int64_t offset, int64_t length) {
    ALOGV("setDataSource(%d, %lld, %lld)", fd, offset, length);
    return OK;
}

status_t FFmpegPlayer::setDataSource(const sp<IStreamSource> &source) {
    return OK;
}

status_t FFmpegPlayer::setVideoSurfaceTexture(
        const sp<IGraphicBufferProducer> &bufferProducer) {
    ALOGV("setVideoSurfaceTexture");
    return OK;
}

status_t FFmpegPlayer::prepare() {
    return OK;
}

status_t FFmpegPlayer::prepareAsync() {
    return OK;
}

status_t FFmpegPlayer::start() {
    ALOGV("start");
    return OK;
}

status_t FFmpegPlayer::stop() {
    ALOGV("stop");
    return OK;
}

status_t FFmpegPlayer::pause() {
    ALOGV("pause");
    return OK;
}

bool FFmpegPlayer::isPlaying() {
    ALOGV("isPlaying");
    return OK;
}

status_t FFmpegPlayer::seekTo(int msec) {
    ALOGV("seekTo %.2f secs", msec / 1E3);
    return OK;
}

status_t FFmpegPlayer::getCurrentPosition(int *msec) {
    ALOGV("getCurrentPosition");
    return OK;
}

status_t FFmpegPlayer::getDuration(int *msec) {
    ALOGV("getDuration");
    return OK;
}

status_t FFmpegPlayer::reset() {
    ALOGV("reset");
    return OK;
}

status_t FFmpegPlayer::setLooping(int loop) {
    ALOGV("setLooping");
    return OK;
}

player_type FFmpegPlayer::playerType() {
    ALOGV("playerType");
    return FFMPEG_PLAYER;
}

status_t FFmpegPlayer::invoke(const Parcel &request, Parcel *reply) {
    ALOGV("invoke()");
    return OK;
}

void FFmpegPlayer::setAudioSink(const sp<AudioSink> &audioSink) {
    ALOGV("setAudioSink");
}

status_t FFmpegPlayer::setParameter(int key, const Parcel &request) {
    ALOGV("setParameter(key=%d)", key);
    return OK;
}

status_t FFmpegPlayer::getParameter(int key, Parcel *reply) {
    ALOGV("getParameter");
    return OK;
}

status_t FFmpegPlayer::getMetadata(
        const media::Metadata::Filter& /* ids */, Parcel *records) {
    return OK;
}

status_t FFmpegPlayer::dump(int fd, const Vector<String16> &args) const {
    return OK;
}

}  // namespace android
