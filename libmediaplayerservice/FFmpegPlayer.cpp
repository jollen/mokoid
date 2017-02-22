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

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <utils/Log.h>
#include <media/AudioTrack.h>

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

status_t FFmpegPlayer::start() {
    if (mTrack != 0) mTrack->start();

/*                                                            
ssize_t     write(const void* buffer, size_t size, bool blocking = true);   
*/                                                            
    while (read(fds[0], b, 1) > 0) {                          
        if (mTrack != 0) mTrack->write(b, 1, true);                                 
    }  

    return OK;
}

status_t FFmpegPlayer::prepare() {
    return OK;
}

void FFmpegPlayer::audioCallback(int event, void *cookie, void *info) {
    ALOGV("audioCallback");
};

status_t FFmpegPlayer::prepareAsync() {
    ALOGV("start");

    sp<AudioTrack> t;

    pid = getpid();
    uid = getuid();

    pipe(fds);

    child = fork();

    if (child != 0) {
        close(fds[0]);

        int fd;
        fd  = open("/tmp/audio", O_RDONLY);

        while (read(fd, &b, 1) >= 0) {
            write(fds[1], &b, 1);
        }

	close(fd);
	close(fds[1]);

	return waitpid(child, NULL, 0);
    }
/*
AudioTrack::AudioTrack(
        audio_stream_type_t streamType,
        uint32_t sampleRate,
        audio_format_t format,
        audio_channel_mask_t channelMask,
        size_t frameCount,
        audio_output_flags_t flags,
        callback_t cbf,
        void* user,
        uint32_t notificationFrames,
        int sessionId,
        transfer_type transferType,
        const audio_offload_info_t *offloadInfo,
        int uid,
        pid_t pid,
        const audio_attributes_t* pAttributes) 
*/
    t = new AudioTrack(
	AUDIO_STREAM_MUSIC, //stream type
	44100, // sample rate
	AUDIO_FORMAT_PCM_16_BIT, // format
	CHANNEL_MASK_USE_CHANNEL_ORDER, // channel mask
	0, // frame count (0: default)
	AUDIO_OUTPUT_FLAG_COMPRESS_OFFLOAD, // flags, see <system/audio.h>
        audioCallback, // callback                             
        NULL, // user data                            
        0, // notification frames                     
        0, // session ID (0: use default)                                   
        AudioTrack::TRANSFER_DEFAULT, // transfer type
        NULL, // offload info    
        uid, // user ID
        pid, // process ID
        NULL); // audio attributes

    mTrack.clear();
    mTrack = t;

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
