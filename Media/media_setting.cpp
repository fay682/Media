#include<Windows.h>
#include "media_setting.h"
extern "C" {
#include "libavformat/avformat.h"
#include "libavutil/log.h"
}

void MediaSetting::SetFfmpegLog() {
    av_log_set_level(AV_LOG_ERROR);
    av_log_set_callback([](void* ptr, int level, const char* fmt, va_list v) {
        char buffer[512];
        if (vsprintf_s(buffer, fmt, v) > 0) {
            int bufSize = MultiByteToWideChar(CP_ACP, 0, buffer, -1, NULL, 0);
            wchar_t* wp = new wchar_t[bufSize];
            MultiByteToWideChar(CP_ACP, 0, buffer, -1, wp, bufSize);
            OutputDebugString(wp);
        }
    });
}

void MediaSetting::GetAudioDataFromMediaFile(std::string const& src, std::string const& dst) {
    int ret = -1;
    int stream_index = -1;
    AVFormatContext* avFormatContextInput = nullptr;
    AVFormatContext* avFormatContextOutput = nullptr;
    AVStream* avStream = nullptr;
    AVCodecParameters* avCodecParametersInput = nullptr;
    AVCodecParameters* avCodecParametersOutput = nullptr;

    ret = avformat_open_input(&avFormatContextInput, src.c_str(), NULL, NULL);
    if (ret != 0) {
        avformat_close_input(&avFormatContextInput);
        avFormatContextInput = nullptr;
    }

    stream_index = av_find_best_stream(avFormatContextInput, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (stream_index < 0) {
        avformat_close_input(&avFormatContextInput);
        avFormatContextInput = nullptr;
    }

    //输出格式上下文->输出格式->创建输出流->参数信息拷贝到输出流中
    avFormatContextOutput = avformat_alloc_context();
    if (!avFormatContextOutput) {
        avformat_close_input(&avFormatContextInput);
        avFormatContextInput = nullptr;
    }
    const AVOutputFormat * avOutputFormat = av_guess_format(NULL, dst.c_str(), NULL);
    if (!avOutputFormat) {
        avformat_close_input(&avFormatContextInput);
        avFormatContextInput = nullptr;
        if (avFormatContextOutput) {
            avformat_free_context(avFormatContextOutput);
            avFormatContextOutput = nullptr;
        }
    }
    avFormatContextOutput->oformat = avOutputFormat;
    avStream = avformat_new_stream(avFormatContextOutput, NULL);
    if (!avStream) {
        avformat_close_input(&avFormatContextInput);
        avFormatContextInput = nullptr;
        if (avFormatContextOutput) {
            avformat_free_context(avFormatContextOutput);
            avFormatContextOutput = nullptr;
        }
    }
    avCodecParametersInput = avFormatContextInput->streams[stream_index]->codecpar;
    avCodecParametersOutput = avStream->codecpar;
    ret = avcodec_parameters_copy(avCodecParametersOutput, avCodecParametersInput);
    if (ret < 0) {
        avformat_close_input(&avFormatContextInput);
        avFormatContextInput = nullptr;
        if (avFormatContextOutput) {
            avformat_free_context(avFormatContextOutput);
            avFormatContextOutput = nullptr;
        }
    }
    avStream->codecpar->codec_tag = 0;
    avFormatContextOutput->nb_streams = 1;







}
