/****************************************************************************
** Copyright (C) 2016 Olaf Japp
**
** This file is part of AnimationMaker.
**
**  AnimationMaker is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  AnimationMaker is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with AnimationMaker.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include "exportmovie.h"

#include <math.h>
#include <QtWidgets>
#include <QImage>
#include <QPainter>
#include <QAbstractAnimation>
#include <QtTest/QTest>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "exception.h"

extern "C"
{
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/avassert.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>
#include <libavutil/timestamp.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}

typedef struct OutputStream
{
    AVStream *st;
    AVCodecContext *enc;
    int64_t next_pts;
    int samples_count;
    AVFrame *frame;
    float t, tincr, tincr2;
    struct SwsContext *sws_ctx;
    struct SwrContext *swr_ctx;
} OutputStream;

static AVFrame *alloc_picture(enum AVPixelFormat pix_fmt, int width, int height)
{
    AVFrame *picture;
    int ret;
    picture = av_frame_alloc();
    if (!picture)
        throw new Exception("Could not allocate frame data.");
    picture->format = pix_fmt;
    picture->width  = width;
    picture->height = height;

    ret = av_frame_get_buffer(picture, 32);
    if (ret < 0)
        throw new Exception("Could not allocate frame data.");

    return picture;
}

ExportMovie::ExportMovie()
{
}

QString ExportMovie::displayName() const
{
    return "Movie";
}

QString ExportMovie::filter() const
{
    return "Video format (*.mpg *.mp4 *.avi);;All Files (*)";
}

QString ExportMovie::title() const
{
    return "Export Animation to Movie";
}

Exception::Exception(QString msg)
{
    m_msg = msg;
}

static void add_stream(OutputStream *ost, AVFormatContext *oc, AVCodec **codec, enum AVCodecID codec_id, int fps, int width, int height)
{
    AVCodecContext *c;

    *codec = avcodec_find_encoder(codec_id);
    if (!(*codec))
        throw new Exception(QString("Could not find encoder for ") + avcodec_get_name(codec_id));

    ost->st = avformat_new_stream(oc, NULL);
    if (!ost->st)
        throw new Exception("Could not allocate stream");

    ost->st->id = oc->nb_streams-1;
    c = avcodec_alloc_context3(*codec);
    if (!c)
        throw new Exception("Could not alloc an encoding context");

    ost->enc = c;
    switch ((*codec)->type)
    {

        case AVMEDIA_TYPE_VIDEO:
            c->codec_id = codec_id;
            c->bit_rate = 1600000;;
            c->width    = width;
            c->height   = height;
            ost->st->time_base.num = 1;
            ost->st->time_base.den = fps;
            c->time_base       = ost->st->time_base;
            c->gop_size      = 12;
            c->pix_fmt       = AV_PIX_FMT_YUV420P;
            if (c->codec_id == AV_CODEC_ID_MPEG2VIDEO)
            {
                c->max_b_frames = 2;
            }
            if (c->codec_id == AV_CODEC_ID_MPEG1VIDEO)
            {
                c->mb_decision = 2;
            }
            break;
        default:
            break;
    }
    if (oc->oformat->flags & AVFMT_GLOBALHEADER)
        c->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
}

static void open_video(AVCodec *codec, OutputStream *ost, AVDictionary *opt_arg)
{
    int ret;
    AVCodecContext *c = ost->enc;
    AVDictionary *opt = NULL;

    av_dict_copy(&opt, opt_arg, 0);
    ret = avcodec_open2(c, codec, &opt);
    av_dict_free(&opt);
    if (ret < 0)
        throw new Exception("Could not open video codec");

    ost->frame = alloc_picture(c->pix_fmt, c->width, c->height);
    if (!ost->frame)
        throw new Exception("Could not allocate video frame");

    ret = avcodec_parameters_from_context(ost->st->codecpar, c);
    if (ret < 0)
        throw new Exception("Could not copy the stream parameters");
}

static AVFrame *get_video_frame(OutputStream *ost, QImage img)
{
    AVCodecContext *c = ost->enc;

    if (av_frame_make_writable(ost->frame) < 0)
        throw new Exception("Could not make frame writeable");

    uint8_t * inData[1] = { img.bits() };
    int inLinesize[1] = { 4 * c->width };

    if (!ost->sws_ctx)
    {
        ost->sws_ctx = sws_getContext(c->width, c->height, AV_PIX_FMT_BGRA, c->width, c->height, c->pix_fmt, SWS_BICUBIC, NULL, NULL, NULL);
        if (!ost->sws_ctx)
            throw new Exception("Could not initialize the conversion context");
    }
    sws_scale(ost->sws_ctx, (const uint8_t * const *)inData, inLinesize, 0, c->height, ost->frame->data, ost->frame->linesize);

    ost->frame->pts = ost->next_pts++;

    return ost->frame;
}

static void close_stream(OutputStream *ost)
{
    avcodec_free_context(&ost->enc);
    av_frame_free(&ost->frame);
    sws_freeContext(ost->sws_ctx);
    swr_free(&ost->swr_ctx);
}

static int write_frame(AVFormatContext *fmt_ctx, const AVRational *time_base, AVStream *st, AVPacket *pkt)
{
    av_packet_rescale_ts(pkt, *time_base, st->time_base);
    pkt->stream_index = st->index;
    return av_interleaved_write_frame(fmt_ctx, pkt);
}

static int write_video_frame(AVFormatContext *oc, OutputStream *ost, QImage img)
{
    int ret;
    AVCodecContext *c;
    AVFrame *frame;
    int got_packet = 0;
    AVPacket pkt = { 0 };
    c = ost->enc;
    frame = get_video_frame(ost, img);
    av_init_packet(&pkt);
    ret = avcodec_encode_video2(c, &pkt, frame, &got_packet);
    if (ret < 0)
        throw new Exception("Error encoding video frame");

    if (got_packet)
    {
        ret = write_frame(oc, &c->time_base, ost->st, &pkt);
    }
    else
    {
        ret = 0;
    }
    if (ret < 0)
        throw new Exception("Error while writing video frame");

    return (frame || got_packet) ? 0 : 1;
}

void ExportMovie::exportMovie(QString filename, QGraphicsView *view, int length, int fps, QStatusBar *bar)
{
    AVFormatContext *oc;
    AVOutputFormat *fmt;
    AVCodec *video_codec;
    OutputStream video_st = { 0 };
    int ret;
    AVDictionary *opt = NULL;

    av_register_all();

    avformat_alloc_output_context2(&oc, NULL, NULL, filename.toLatin1());
    if (!oc)
    {
        //Could not deduce output format from file extension: using MPEG.
        avformat_alloc_output_context2(&oc, NULL, "mpeg", filename.toLatin1());
    }
    if (!oc)
        throw new Exception("Unable to allocate context");
    fmt = oc->oformat;
    if (fmt->video_codec != AV_CODEC_ID_NONE)
    {
        add_stream(&video_st, oc, &video_codec, fmt->video_codec, fps, view->width(), view->height());
    }

    open_video(video_codec, &video_st, opt);
    if (!(fmt->flags & AVFMT_NOFILE))
    {
        ret = avio_open(&oc->pb, filename.toLatin1(), AVIO_FLAG_WRITE);
        if (ret < 0)
            throw new Exception(QString("Could not open '%1'").arg(filename));

    }
    ret = avformat_write_header(oc, &opt);
    if (ret < 0)
        throw new Exception("Error occurred when opening output file");

    int delay = 1000 / fps;
    int frames = length / delay + 2;

    for (int i = 0; i < frames; i++)
    {
        bar->showMessage(QString("Writing frame %1 of %2 frames").arg(i).arg(frames));

        emit setFrame(i * delay);
        QTest::qSleep(delay);
        QCoreApplication::processEvents(QEventLoop::AllEvents, delay);

        QImage img = view->grab().toImage();

        write_video_frame(oc, &video_st, img);
    }
    av_write_trailer(oc);
    close_stream(&video_st);
    if (!(fmt->flags & AVFMT_NOFILE))
        avio_closep(&oc->pb);
    avformat_free_context(oc);
    bar->showMessage(QString("Ready"));
}
