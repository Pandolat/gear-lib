/******************************************************************************
 * Copyright (C) 2014-2020 Zhifeng Gong <gozfree@163.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/
#include "libmedia-io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct media_packet *media_packet_create(enum media_type type, void *data, size_t len)
{
    struct media_packet *packet;
    packet = calloc(1, sizeof(struct media_packet));
    if (!packet) {
        return NULL;
    }
    packet->type = type;
    switch (packet->type) {
    case MEDIA_TYPE_AUDIO:
        packet->audio = audio_packet_create(data, len);
        break;
    case MEDIA_TYPE_VIDEO:
        packet->video = video_packet_create(data, len);
        break;
    default:
        printf("unsupport create %d media packet\n", packet->type);
        break;
    }
    return packet;
}

void media_packet_destroy(struct media_packet *packet)
{
    if (!packet) {
        return;
    }
    switch (packet->type) {
    case MEDIA_TYPE_AUDIO:
        audio_packet_destroy(packet->audio);
        break;
    case MEDIA_TYPE_VIDEO:
        video_packet_destroy(packet->video);
        break;
    default:
        printf("unsupport destroy %d media packet\n", packet->type);
        break;
    }
    free(packet);
}

struct media_packet *media_packet_copy(const struct media_packet *src)
{
    if (!src)
        return NULL;

    struct media_packet *dst = NULL;
    switch (src->type) {
    case MEDIA_TYPE_VIDEO:
        dst = media_packet_create(MEDIA_TYPE_VIDEO, NULL, 0);
        memcpy(dst->video, src->video, sizeof(struct video_packet));
        dst->video->data = calloc(1, src->video->size);
        memcpy(dst->video->data, src->video->data, src->video->size);
        break;
    case MEDIA_TYPE_AUDIO:
        dst = media_packet_create(MEDIA_TYPE_AUDIO, NULL, 0);
        memcpy(dst->audio, src->audio, sizeof(struct audio_packet));
        dst->audio->data = calloc(1, src->audio->size);
        memcpy(dst->audio->data, src->audio->data, src->audio->size);
        break;
    default:
        printf("unsupport copy %d media packet\n", src->type);
        break;
    }
    return dst;
}

