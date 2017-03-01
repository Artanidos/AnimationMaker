#include <math.h>
#include "mainwindow.h"
#include <QtWidgets>
#include <QImage>
#include <QPainter>
#include <QAbstractAnimation>
#include <QtTest/QTest>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
#include <libavutil/mathematics.h>
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
        return NULL;
    picture->format = pix_fmt;
    picture->width  = width;
    picture->height = height;
    ret = av_frame_get_buffer(picture, 32);
    if (ret < 0)
    {
        fprintf(stderr, "Could not allocate frame data.\n");
        exit(1);
    }
    return picture;
}

static void add_stream(OutputStream *ost, AVFormatContext *oc, AVCodec **codec, enum AVCodecID codec_id, int fps, int width, int height)
{
    AVCodecContext *c;

    *codec = avcodec_find_encoder(codec_id);
    if (!(*codec))
    {
        fprintf(stderr, "Could not find encoder for '%s'\n", avcodec_get_name(codec_id));
        exit(1);
    }
    ost->st = avformat_new_stream(oc, NULL);
    if (!ost->st)
    {
        fprintf(stderr, "Could not allocate stream\n");
        exit(1);
    }
    ost->st->id = oc->nb_streams-1;
    c = avcodec_alloc_context3(*codec);
    if (!c)
    {
        fprintf(stderr, "Could not alloc an encoding context\n");
        exit(1);
    }
    ost->enc = c;
    switch ((*codec)->type)
    {

        case AVMEDIA_TYPE_VIDEO:
            c->codec_id = codec_id;
            c->bit_rate = 1600000;//400000;
            c->width    = width;
            c->height   = height;
            ost->st->time_base = (AVRational){ 1, fps };
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
    {
        fprintf(stderr, "Could not open video codec\n");
        exit(1);
    }
    ost->frame = alloc_picture(c->pix_fmt, c->width, c->height);
    if (!ost->frame)
    {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }

    ret = avcodec_parameters_from_context(ost->st->codecpar, c);
    if (ret < 0)
    {
        fprintf(stderr, "Could not copy the stream parameters\n");
        exit(1);
    }
}

static AVFrame *get_video_frame(OutputStream *ost, QImage img)
{
    AVCodecContext *c = ost->enc;

    if (av_frame_make_writable(ost->frame) < 0)
        exit(1);

    uint8_t * inData[1] = { img.bits() };
    int inLinesize[1] = { 4 * c->width };

    if (!ost->sws_ctx)
    {
        ost->sws_ctx = sws_getContext(c->width, c->height, AV_PIX_FMT_BGRA, c->width, c->height, c->pix_fmt, SWS_BICUBIC, NULL, NULL, NULL);
        if (!ost->sws_ctx)
        {
            fprintf(stderr,"Could not initialize the conversion context\n");
            exit(1);
        }
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
    {
        fprintf(stderr, "Error encoding video frame\n");
        exit(1);
    }
    if (got_packet)
    {
        ret = write_frame(oc, &c->time_base, ost->st, &pkt);
    }
    else
    {
        ret = 0;
    }
    if (ret < 0)
    {
        fprintf(stderr, "Error while writing video frame\n");
        exit(1);
    }
    return (frame || got_packet) ? 0 : 1;
}

int video_encode(const char *filename, QGraphicsView *view, QParallelAnimationGroup *group, int fps, int length, MainWindow *win)
{
    AVFormatContext *oc;
    AVOutputFormat *fmt;
    AVCodec *video_codec;
    OutputStream video_st = { 0 };
    int ret;
    AVDictionary *opt = NULL;

    av_register_all();

    avformat_alloc_output_context2(&oc, NULL, NULL, filename);
    if (!oc)
    {
        printf("Could not deduce output format from file extension: using MPEG.\n");
        avformat_alloc_output_context2(&oc, NULL, "mpeg", filename);
    }
    if (!oc)
        return 1;
    fmt = oc->oformat;
    if (fmt->video_codec != AV_CODEC_ID_NONE)
    {
        add_stream(&video_st, oc, &video_codec, fmt->video_codec, fps, view->width(), view->height());
    }

    open_video(video_codec, &video_st, opt);
    if (!(fmt->flags & AVFMT_NOFILE))
    {
        ret = avio_open(&oc->pb, filename, AVIO_FLAG_WRITE);
        if (ret < 0)
        {
            fprintf(stderr, "Could not open '%s'\n", filename);
            return 1;
        }
    }
    ret = avformat_write_header(oc, &opt);
    if (ret < 0)
    {
        fprintf(stderr, "Error occurred when opening output file\n");
        return 1;
    }

    int total = group->totalDuration() > length * 1000 ? group->totalDuration() : length * 1000;
    int delay = 1000 / fps;
    int frames = total / delay + 2;

    group->start();
    group->pause();

    for (int i = 0; i < frames; i++)
    {
        win->statusBar()->showMessage(QString("Writing frame %1 of %2 frames").arg(i).arg(frames));

        group->setCurrentTime(i * delay);
        QTest::qSleep(40);
        QCoreApplication::processEvents(QEventLoop::AllEvents, 40);

        QImage img = view->grab().toImage();

        write_video_frame(oc, &video_st, img);
    }
    av_write_trailer(oc);
    close_stream(&video_st);
    if (!(fmt->flags & AVFMT_NOFILE))
        avio_closep(&oc->pb);
    avformat_free_context(oc);
    win->statusBar()->showMessage(QString("Ready"));
    return 0;
}

/*
int encode(AVCodecContext *avctx, AVPacket *pkt, AVFrame *frame, int *got_packet)
{
    int ret;

    *got_packet = 0;

    ret = avcodec_send_frame(avctx, frame);
    if (ret < 0)
        return ret == AVERROR_EOF ? 0 : ret;

    ret = avcodec_receive_packet(avctx, pkt);
    if (!ret)
        *got_packet = 1;
    if (ret == AVERROR(EAGAIN))
        return 0;

    return ret;
}

void video_encode(const char *filename, QGraphicsView *view, QParallelAnimationGroup *group, int fps, int length, MainWindow *win)
{
    int width = view->width();
    int height = view->height();

    AVCodec *codec;
    AVCodecContext *c= NULL;
    int i, ret, got_output;
    FILE *f;
    AVFrame *frame;
    AVPacket pkt;
    uint8_t endcode[] = { 0, 0, 1, 0xb7 };
    int codec_id = AV_CODEC_ID_MPEG4;

    avcodec_register_all();

    codec = avcodec_find_encoder((AVCodecID)codec_id);
    if (!codec)
    {
        fprintf(stderr, "Codec not found\n");
        return;
    }

    c = avcodec_alloc_context3(codec);
    if (!c)
    {
        av_free(c);
        fprintf(stderr, "Could not allocate video codec context\n");
        return;
    }

    c->bit_rate = 1600000;
    c->width = width;
    c->height = height;
    c->time_base = (AVRational){1,fps};
    c->gop_size = 10;
    c->max_b_frames = 1;
    c->pix_fmt = AV_PIX_FMT_YUV420P;

    if (codec_id == AV_CODEC_ID_H264)
        av_opt_set(c->priv_data, "preset", "slow", 0);

    if (avcodec_open2(c, codec, NULL) < 0)
    {
        av_free(c);
        fprintf(stderr, "Could not open codec\n");
        return;
    }

    f = fopen(filename, "wb");
    if (!f)
    {
        avcodec_close(c);
        av_free(c);
        fprintf(stderr, "Could not open %s\n", filename);
        return;
    }

    frame = av_frame_alloc();
    if (!frame)
    {
        avcodec_close(c);
        av_free(c);
        fclose(f);
        fprintf(stderr, "Could not allocate video frame\n");
        return;
    }
    frame->format = c->pix_fmt;
    frame->width  = c->width;
    frame->height = c->height;

    ret = av_image_alloc(frame->data, frame->linesize, c->width, c->height, c->pix_fmt, 32);
    if (ret < 0)
    {
        avcodec_close(c);
        av_free(c);
        fclose(f);
        av_frame_free(&frame);
        fprintf(stderr, "Could not allocate raw picture buffer\n");
        return;
    }

    struct SwsContext * ctx = sws_getContext(c->width, c->height, AV_PIX_FMT_BGRA, c->width, c->height, AV_PIX_FMT_YUV420P, 0, 0, 0, 0);

    int total = group->totalDuration() > length * 1000 ? group->totalDuration() : length * 1000;
    int delay = 1000 / fps;
    int frames = total / delay + 2;

    group->start();
    group->pause();

    for (i = 0; i < frames; i++)
    {
        group->setCurrentTime(i * delay);
        QTest::qSleep(40);
        QCoreApplication::processEvents(QEventLoop::AllEvents, 40);

        QImage img = view->grab().toImage();

        av_init_packet(&pkt);
        pkt.data = NULL;
        pkt.size = 0;

        fflush(stdout);

        uint8_t * inData[1] = { img.bits() };
        int inLinesize[1] = { 4*c->width };
        sws_scale(ctx, inData, inLinesize, 0, c->height, frame->data, frame->linesize);

        frame->pts = i;

        ret = encode(c, &pkt, frame, &got_output);
        if (ret < 0)
        {
            avcodec_close(c);
            av_free(c);
            fclose(f);
            av_freep(&frame->data[0]);
            av_frame_free(&frame);
            fprintf(stderr, "Error encoding frame\n");
            return;
        }

        if (got_output)
        {
            win->statusBar()->showMessage(QString("Writing frame %1 of %2 frames").arg(i).arg(frames));
            fwrite(pkt.data, 1, pkt.size, f);
            av_packet_unref(&pkt);
        }
    }


    for (got_output = 1; got_output; i++)
    {
        fflush(stdout);

        ret = encode(c, &pkt, NULL, &got_output);
        if (ret < 0)
        {
            avcodec_close(c);
            av_free(c);
            fclose(f);
            av_freep(&frame->data[0]);
            av_frame_free(&frame);
            fprintf(stderr, "Error encoding frame\n");
            return;
        }

        if (got_output)
        {
            win->statusBar()->showMessage(QString("Writing frame %1 of %2 frames").arg(i).arg(frames));
            fwrite(pkt.data, 1, pkt.size, f);
            av_packet_unref(&pkt);
        }
    }

    fwrite(endcode, 1, sizeof(endcode), f);
    fclose(f);

    avcodec_close(c);
    av_free(c);
    av_freep(&frame->data[0]);
    av_frame_free(&frame);
    fflush(stdout);

    group->stop();

    win->statusBar()->showMessage("Ready");
}
*/
