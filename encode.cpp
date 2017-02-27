#include <math.h>
#include "mainwindow.h"
#include <QtWidgets>
#include <QImage>
#include <QPainter>
#include <QAbstractAnimation>
#include <QtTest/QTest>


extern "C"
{
#include <libavutil/opt.h>
#include <libavcodec/avcodec.h>
#include <libavutil/channel_layout.h>
#include <libavutil/common.h>
#include <libavutil/imgutils.h>
#include <libavutil/mathematics.h>
#include <libavutil/samplefmt.h>
#include <libswscale/swscale.h>
}

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
