#pragma once
#include<string>
extern "C" {
#include "libavformat/avformat.h"
#include "libavutil/log.h"
}

void GetAudioDataFromMediaFile(std::string const& src, std::string const& dst) {
    av_log_set_level(AV_LOG_DEBUG);
    AVFormatContext* avFmtCtx = NULL;
    AVStream* outStream = NULL;
    AVStream* inStream = NULL;
    AVOutputFormat* outFmt = NULL;
    AVPacket pkt;
    AVFormatContext* outFmtCtx = NULL;
    int streamIndex = -1;
    int ret;
    ret = avformat_open_input(&avFmtCtx, src.c_str(), NULL, NULL);
    if (ret != 0) {
        av_log(NULL, AV_LOG_ERROR, "call avformat_open_input error!");
        goto _ERROR;
    }

    streamIndex = av_find_best_stream(avFmtCtx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (streamIndex < 0) {
        av_log(NULL, AV_LOG_ERROR, "call av_find_best_stream error!");
        goto _ERROR;
    }
    //目的文件上下文
    outFmtCtx = avformat_alloc_context();
    if (!outFmtCtx) {
        av_log(NULL, AV_LOG_ERROR, "call avformat_alloc_context error!");
        goto _ERROR;
    }
    outFmt = const_cast<AVOutputFormat*>(av_guess_format(NULL, dst.c_str(), NULL));
    outFmtCtx->oformat = outFmt;
    //创建输出流
    outStream = avformat_new_stream(outFmtCtx, NULL);
    inStream = avFmtCtx->streams[streamIndex];
    ret = avcodec_parameters_copy(outStream->codecpar, inStream->codecpar);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "call avcodec_parameters_copy error!");
        goto _ERROR;
    }
    outStream->codecpar->codec_tag = 0;//[可以根据多媒体文件自动匹配编解码器]
    ret = avio_open2(&outFmtCtx->pb, dst.c_str(), AVIO_FLAG_WRITE, NULL, NULL); //[上下文与目的文件绑定]
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "call avio_open2 error!");
        goto _ERROR;
    }
    outFmtCtx->streams[0]->codecpar->codec_type = AVMEDIA_TYPE_AUDIO;
    ret = avformat_write_header(outFmtCtx, NULL);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "call avformat_write_header error!");
        goto _ERROR;
    }

    while (av_read_frame(avFmtCtx, &pkt) >= 0) {
        if (pkt.stream_index == streamIndex) {//[读取的数据是否为音频数据]
            pkt.pts = av_rescale_q_rnd(pkt.pts, inStream->time_base, outStream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
            pkt.dts = pkt.pts;
            pkt.duration = av_rescale_q(pkt.duration, inStream->time_base, outStream->time_base);
            pkt.stream_index = 0;//[输出文件只有一路音频流]
            pkt.pos = -1;//[自动匹配一帧音频的相对位置]
            av_interleaved_write_frame(outFmtCtx, &pkt);
            av_packet_unref(&pkt);
        }
    }
    av_write_trailer(outFmtCtx);

_ERROR:
    if (avFmtCtx) {
        avformat_close_input(&avFmtCtx);
        avFmtCtx = NULL;
    }
    if (outFmtCtx->pb) {//[说明打开了目标文件]
        avio_close(outFmtCtx->pb);
    }
    if (outFmtCtx) {
        avformat_free_context(outFmtCtx);
    }

}


void GetVideoDataFromMediaFile(std::string const& src, std::string const& dst) {
    av_log_set_level(AV_LOG_DEBUG);
    AVFormatContext* avFmtCtx = NULL;
    AVStream* outStream = NULL;
    AVStream* inStream = NULL;
    AVOutputFormat* outFmt = NULL;
    AVPacket pkt;
    AVFormatContext* outFmtCtx = NULL;
    int streamIndex = -1;
    int ret;
    ret = avformat_open_input(&avFmtCtx, src.c_str(), NULL, NULL);
    if (ret != 0) {
        av_log(NULL, AV_LOG_ERROR, "call avformat_open_input error!");
        goto _ERROR;
    }

    streamIndex = av_find_best_stream(avFmtCtx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if (streamIndex < 0) {
        av_log(NULL, AV_LOG_ERROR, "call av_find_best_stream error!");
        goto _ERROR;
    }
    //目的文件上下文
    outFmtCtx = avformat_alloc_context();
    if (!outFmtCtx) {
        av_log(NULL, AV_LOG_ERROR, "call avformat_alloc_context error!");
        goto _ERROR;
    }
    outFmt = const_cast<AVOutputFormat*>(av_guess_format(NULL, dst.c_str(), NULL));
    outFmtCtx->oformat = outFmt;
    //创建输出流
    outStream = avformat_new_stream(outFmtCtx, NULL);
    inStream = avFmtCtx->streams[streamIndex];
    avcodec_parameters_copy(outStream->codecpar, inStream->codecpar);
    outStream->codecpar->codec_tag = 0;//[可以根据多媒体文件自动匹配编解码器]
    ret = avio_open2(&outFmtCtx->pb, dst.c_str(), AVIO_FLAG_WRITE, NULL, NULL);//[上下文与目的文件绑定]
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "call avio_open2 error!");
        goto _ERROR;
    }

    ret = avformat_write_header(outFmtCtx, NULL);
    if (ret < 0) {
        av_log(NULL, AV_LOG_ERROR, "call avformat_write_header error!");
        goto _ERROR;
    }

    while (av_read_frame(avFmtCtx, &pkt) >= 0) {
        if (pkt.stream_index == streamIndex) {//[读取的数据是否为音频数据]
            pkt.pts = av_rescale_q_rnd(pkt.pts, inStream->time_base, outStream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
            pkt.dts = av_rescale_q_rnd(pkt.dts, inStream->time_base, outStream->time_base, (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
            pkt.duration = av_rescale_q(pkt.duration, inStream->time_base, outStream->time_base);
            pkt.stream_index = 0;//[输出文件只有一路音频流]
            pkt.pos = -1;//[自动匹配一帧音频的相对位置]
            av_interleaved_write_frame(outFmtCtx, &pkt);
            av_packet_unref(&pkt);
        }
    }
    av_write_trailer(outFmtCtx);

_ERROR:
    if (avFmtCtx) {
        avformat_close_input(&avFmtCtx);
        avFmtCtx = NULL;
    }
    if (outFmtCtx->pb) {//[说明打开了目标文件]
        avio_close(outFmtCtx->pb);
    }
    if (outFmtCtx) {
        avformat_free_context(outFmtCtx);
    }

}