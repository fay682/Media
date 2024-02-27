#define _CRT_SECURE_NO_WARNINGS
#include "video_decode_play_thread.h"
#include <fstream>
#include<Windows.h>


std::string url = "test.yuv";
void SaveFrame(AVFrame* frame, AVCodecContext* codec_context) {
    AVFrame* srcframe = frame;
    AVPixelFormat srcFormat = codec_context->pix_fmt;
    auto dstFrameYuv = av_frame_alloc();
    /*align:此参数是设定内存对齐的对齐数，也就是按多大的字节进行内存对齐。比如设置为1，表示按1字节对齐，那么得到的结果就是与实际的内存大小一样。
    再比如设置为4，表示按4字节对齐。也就是内存的起始地址必须是4的整倍数。*/
    auto memey_size = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, srcframe->width, srcframe->height, 1);
    auto dst_buffer = (unsigned char*)av_malloc(memey_size);
    av_image_fill_arrays(dstFrameYuv->data, dstFrameYuv->linesize, dst_buffer,
                         AV_PIX_FMT_YUV420P, srcframe->width, srcframe->height, 1);
    auto sws_context = sws_getContext(srcframe->width, srcframe->height, srcFormat,
                                      srcframe->width, srcframe->height, AV_PIX_FMT_YUV420P, SWS_FAST_BILINEAR, NULL, NULL, NULL);

    sws_scale(sws_context, (const unsigned char* const*)srcframe->data, srcframe->linesize, 0, srcframe->height,
              dstFrameYuv->data, dstFrameYuv->linesize);

    std::string file_path = url;
    FILE* fp_yuv = fopen(file_path.c_str(), "ab+");
    if (fp_yuv) {
        fwrite(dstFrameYuv->data[0], 1, (srcframe->width) * (srcframe->height), fp_yuv);    //Y
        fwrite(dstFrameYuv->data[1], 1, (srcframe->width) * (srcframe->height) / 4, fp_yuv);  //V
        fwrite(dstFrameYuv->data[2], 1, (srcframe->width) * (srcframe->height) / 4, fp_yuv);  //U
        fclose(fp_yuv);
    }
    av_frame_free(&dstFrameYuv);
}

void VideoDecodePlayThread::Run() {
    while (!is_exist()) {
        mutex_.lock();
        AVPacket* avPacket = Pop();
        int ret = decode_->Send(avPacket);
        if (!ret) {
            mutex_.unlock();
            Sleep(1);
            continue;
        }
        while (!is_exist()) {
            AVFrame* avFrame = decode_->Receive();
            if (!avFrame) {
                break;
            }
            //渲染该视频数据
            //保存数据帧
            SaveFrame(avFrame, decode_->avCodecContext_);
            //释放
            av_frame_free(&avFrame);
        }
        mutex_.unlock();
    }
}

void VideoDecodePlayThread::Open(AVCodecParameters * avCodecParameters) {
    mutex_.lock();
    decode_->Open(avCodecParameters);
    mutex_.unlock();
}

void VideoDecodePlayThread::Close() {

}