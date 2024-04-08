#include<Windows.h>
#include "media_setting.h"


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

void MediaSetting::ReleaseSource() {
    if (avFrame_) {
        av_frame_free(&avFrame_);
        avFrame_ = nullptr;
    }
    if (avStream_index_) {
        av_free(avStream_index_);
        avStream_index_ = nullptr;
    }
    if (avStream_seek_pts_) {
        av_free(avStream_seek_pts_);
        avStream_seek_pts_ = nullptr;
    }
    if (avStream_seek_dts_) {
        av_free(avStream_seek_dts_);
        avStream_seek_dts_ = nullptr;
    }
    if (avFormatContextInput_) {
        avformat_close_input(&avFormatContextInput_);
        avFormatContextInput_ = nullptr;
    }
    if (avFormatContextOutput_) {
        if (avFormatContextOutput_->pb) {
            avio_close(avFormatContextOutput_->pb);
        }
        avformat_free_context(avFormatContextOutput_);
        avFormatContextOutput_ = nullptr;
    }
}

void MediaSetting::GetAudioDataFromMediaFile(std::string const& src, std::string const& dst) {
    int ret = -1;
    ret = avformat_open_input(&avFormatContextInput_, src.c_str(), NULL, NULL);
    if (ret != 0) {
        ReleaseSource();
    }

    stream_index_ = av_find_best_stream(avFormatContextInput_, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (stream_index_ < 0) {
        ReleaseSource();
    }

    //输出格式上下文->输出格式->创建输出流->参数信息拷贝到输出流中
    avFormatContextOutput_ = avformat_alloc_context();
    if (!avFormatContextOutput_) {
        ReleaseSource();
    }
    const AVOutputFormat * avOutputFormat = av_guess_format(NULL, dst.c_str(), NULL);
    if (!avOutputFormat) {
        ReleaseSource();
    }
    avFormatContextOutput_->oformat = avOutputFormat;
    avStream_ = avformat_new_stream(avFormatContextOutput_, NULL);
    if (!avStream_) {
        ReleaseSource();
    }
    avCodecParametersInput_ = avFormatContextInput_->streams[stream_index_]->codecpar;
    avCodecParametersOutput_ = avStream_->codecpar;
    ret = avcodec_parameters_copy(avCodecParametersOutput_, avCodecParametersInput_);
    if (ret < 0) {
        ReleaseSource();
    }
    avStream_->codecpar->codec_tag = 0;

    ret = avio_open2(&avFormatContextOutput_->pb, dst.c_str(), AVIO_FLAG_WRITE, nullptr, nullptr);
    if (ret < 0) {
        ReleaseSource();
    }

    ret = avformat_write_header(avFormatContextOutput_, nullptr);
    if (ret < 0) {
        ReleaseSource();
    }

    avFormatContextOutput_->streams[0]->codecpar->codec_type = AVMEDIA_TYPE_AUDIO;

    while (av_read_frame(avFormatContextInput_, &avPacket_) >= 0) {
        if (avPacket_.stream_index == stream_index_) {
            avPacket_.pts = av_rescale_q_rnd(avPacket_.pts, avFormatContextInput_->streams[stream_index_]->time_base, avStream_->time_base,
                                             (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
            avPacket_.dts = avPacket_.pts;
            avPacket_.duration = av_rescale_q(avPacket_.duration, avFormatContextInput_->streams[stream_index_]->time_base, avStream_->time_base);
            avPacket_.stream_index = 0;
            avPacket_.pos = -1;
            av_interleaved_write_frame(avFormatContextOutput_, &avPacket_);
        }
        av_packet_unref(&avPacket_);
    }
    av_write_trailer(avFormatContextOutput_);

    ReleaseSource();
}

void MediaSetting::GetVideoDataFromMediaFile(std::string const& src, std::string const& dst) {
    int ret = -1;
    ret = avformat_open_input(&avFormatContextInput_, src.c_str(), nullptr, nullptr);
    if (ret != 0) {
        ReleaseSource();
    }

    stream_index_ = av_find_best_stream(avFormatContextInput_, AVMEDIA_TYPE_VIDEO, -1, -1, nullptr, 0);
    if (stream_index_ < 0) {
        ReleaseSource();
    }

    avFormatContextOutput_ = avformat_alloc_context();
    if (!avFormatContextOutput_) {
        ReleaseSource();
    }
    const AVOutputFormat* avOutputFormat = av_guess_format(nullptr, dst.c_str(), nullptr);
    avFormatContextOutput_->oformat = avOutputFormat;
    avStream_ = avformat_new_stream(avFormatContextOutput_, nullptr);
    if (!avStream_) {
        ReleaseSource();
    }

    avCodecParametersInput_ = avFormatContextInput_->streams[stream_index_]->codecpar;
    avCodecParametersOutput_ = avStream_->codecpar;
    ret = avcodec_parameters_copy(avCodecParametersOutput_, avCodecParametersInput_);
    if (ret < 0) {
        ReleaseSource();
    }

    avStream_->codecpar->codec_tag = 0;


    ret = avio_open2(&avFormatContextOutput_->pb, dst.c_str(), AVIO_FLAG_WRITE, nullptr, nullptr);
    if (ret < 0) {
        ReleaseSource();
    }

    //调用之后
    avformat_write_header(avFormatContextOutput_, nullptr);

    avFormatContextOutput_->streams[0]->codecpar->codec_type = AVMEDIA_TYPE_VIDEO;

    while (av_read_frame(avFormatContextInput_, &avPacket_) >= 0) {
        if (avPacket_.stream_index == stream_index_) {
            avPacket_.pts = av_rescale_q_rnd(avPacket_.pts, avFormatContextInput_->streams[stream_index_]->time_base, avStream_->time_base,
                                             (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
            avPacket_.dts = av_rescale_q_rnd(avPacket_.dts, avFormatContextInput_->streams[stream_index_]->time_base, avStream_->time_base,
                                             (AVRounding)(AV_ROUND_NEAR_INF | AV_ROUND_PASS_MINMAX));
            avPacket_.duration = av_rescale_q(avPacket_.duration, avFormatContextInput_->streams[stream_index_]->time_base, avStream_->time_base);
            avPacket_.stream_index = 0;
            avPacket_.pos = -1;
            av_interleaved_write_frame(avFormatContextOutput_, &avPacket_);
        }
        av_packet_unref(&avPacket_);
    }
    av_write_trailer(avFormatContextOutput_);
    ReleaseSource();

}

void MediaSetting::RemuxMediaFile(std::string const& src, std::string const& dst) {
    int ret = -1;
    ret = avformat_open_input(&avFormatContextInput_, src.c_str(), nullptr, nullptr);
    if (ret != 0) {
        ReleaseSource();
    }

    avFormatContextOutput_ = avformat_alloc_context();
    if (!avFormatContextOutput_) {
        ReleaseSource();
    }
    const AVOutputFormat* avOutputFormat = av_guess_format(nullptr, dst.c_str(), nullptr);
    avFormatContextOutput_->oformat = avOutputFormat;

    avStream_index_ = (int *)av_calloc(avFormatContextInput_->nb_streams, sizeof(int));
    int avStream_index_count = -1;
    for (int i = 0; i < avFormatContextInput_->nb_streams; i++) {
        if (avFormatContextInput_->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO ||
                avFormatContextInput_->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO ||
                avFormatContextInput_->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_SUBTITLE) {
            ++avStream_index_count;
            avStream_index_[i] = avStream_index_count;
            avStream_ = avformat_new_stream(avFormatContextOutput_, nullptr);
            if (!avStream_) {
                ReleaseSource();
            }
            ret = avcodec_parameters_copy(avStream_->codecpar, avFormatContextInput_->streams[i]->codecpar);
            if (ret < 0) {
                ReleaseSource();
            }
            avStream_->codecpar->codec_tag = 0;
        } else {
            avStream_index_[i] = -1;
        }
    }

    ret = avio_open2(&avFormatContextOutput_->pb, dst.c_str(), AVIO_FLAG_WRITE, nullptr, nullptr);
    if (ret < 0) {
        ReleaseSource();
    }

    avformat_write_header(avFormatContextOutput_, nullptr);

    while (av_read_frame(avFormatContextInput_, &avPacket_) >= 0) {
        if (avStream_index_[avPacket_.stream_index] >= 0) {
            av_packet_rescale_ts(&avPacket_, avFormatContextInput_->streams[avPacket_.stream_index]->time_base, avFormatContextOutput_->streams[avPacket_.stream_index]->time_base);
            avPacket_.stream_index = avStream_index_[avPacket_.stream_index];
            avPacket_.pos = -1;
            av_interleaved_write_frame(avFormatContextOutput_, &avPacket_);
        }
        av_packet_unref(&avPacket_);
    }
    av_write_trailer(avFormatContextOutput_);
    ReleaseSource();

}

void MediaSetting::CutMediaFile(const std::string& src, const std::string& dst, double start_time, double end_time) {
    int ret = -1;
    ret = avformat_open_input(&avFormatContextInput_, src.c_str(), nullptr, nullptr);
    if (ret != 0) {
        ReleaseSource();
    }

    avFormatContextOutput_ = avformat_alloc_context();
    if (!avFormatContextOutput_) {
        ReleaseSource();
    }

    const AVOutputFormat* avOutputFormat = av_guess_format(nullptr, dst.c_str(), nullptr);
    if (!avOutputFormat) {
        ReleaseSource();
    }
    avFormatContextOutput_->oformat = avOutputFormat;

    ret = av_seek_frame(avFormatContextInput_, -1, start_time * AV_TIME_BASE, AVSEEK_FLAG_BACKWARD);
    if (ret < 0) {
        ReleaseSource();
    }


    avStream_index_ = (int*)av_calloc(avFormatContextInput_->nb_streams, sizeof(int));
    avStream_seek_pts_ = (int*)av_calloc(avFormatContextInput_->nb_streams, sizeof(int));
    avStream_seek_dts_ = (int*)av_calloc(avFormatContextInput_->nb_streams, sizeof(int));
    stream_index_ = -1;
    for (int i = 0; i < avFormatContextInput_->nb_streams; i++) {
        if (avFormatContextInput_->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO ||
                avFormatContextInput_->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO ||
                avFormatContextInput_->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_SUBTITLE) {
            ++stream_index_;
            avStream_index_[i] = stream_index_;
            avStream_ = avformat_new_stream(avFormatContextOutput_, nullptr);
            if (!avStream_) {
                ReleaseSource();
            }
            ret = avcodec_parameters_copy(avStream_->codecpar, avFormatContextInput_->streams[i]->codecpar);
            if (ret < 0) {
                ReleaseSource();
            }
            avStream_->codecpar->codec_tag = 0;
        } else {
            avStream_index_[i] = -1;
        }
        avStream_seek_pts_[i] = -1;
        avStream_seek_dts_[i] = -1;
    }

    ret = avio_open2(&avFormatContextOutput_->pb, dst.c_str(), AVIO_FLAG_WRITE, nullptr, nullptr);
    if (ret < 0) {
        ReleaseSource();
    }

    ret = avformat_write_header(avFormatContextOutput_, nullptr);
    if (ret != 0) {
        ReleaseSource();
    }

    while (av_read_frame(avFormatContextInput_, &avPacket_) >= 0) {
        if (avPacket_.pts > 0 && avStream_seek_pts_[avPacket_.stream_index] == -1) {
            avStream_seek_pts_[avPacket_.stream_index] = avPacket_.pts;
        }
        if (avPacket_.dts > 0 && avStream_seek_dts_[avPacket_.stream_index] == -1) {
            avStream_seek_dts_[avPacket_.stream_index] = avPacket_.dts;
        }
        if ((avPacket_.pts * av_q2d(avFormatContextInput_->streams[avPacket_.stream_index]->time_base)) > end_time ) {
            av_packet_unref(&avPacket_);
            break;
        }
        if (avStream_index_[avPacket_.stream_index] >= 0) {
            avPacket_.pts = avPacket_.pts - avStream_seek_pts_[avPacket_.stream_index];
            avPacket_.dts = avPacket_.dts - avStream_seek_dts_[avPacket_.stream_index];
            if (avPacket_.dts > avPacket_.pts) {
                avPacket_.pts = avPacket_.dts;
            }
            av_packet_rescale_ts(&avPacket_, avFormatContextInput_->streams[avPacket_.stream_index]->time_base, avFormatContextOutput_->streams[avPacket_.stream_index]->time_base);
            avPacket_.stream_index = avStream_index_[avPacket_.stream_index];
            avPacket_.pos = -1;
            av_interleaved_write_frame(avFormatContextOutput_, &avPacket_);
        }
        av_packet_unref(&avPacket_);
    }

    ret = av_write_trailer(avFormatContextOutput_);
    ReleaseSource();

}

void MediaSetting::CreateYuvVideoFile(const std::string& src) {
    int ret = -1;
    int width = 640;
    int height = 380;

    FILE* file_ptr;
    fopen_s(&file_ptr, src.c_str(), "ab+");


    avFrame_ = av_frame_alloc();

    //av_frame_get_buffer调用前需要设置的参数
    avFrame_->width = width;
    avFrame_->height = height;
    avFrame_->format = AV_PIX_FMT_YUV420P;

    ret = av_frame_get_buffer(avFrame_, 0);
    if (ret != 0) {
        ReleaseSource();
    }

    uint8_t* yPlaneData = avFrame_->data[0];
    uint8_t* uPlaneData = avFrame_->data[1];
    uint8_t* vPlaneData = avFrame_->data[2];

    int frame_rate = 25;
    //3min时长
    for (int i = 0; i < frame_rate * 3; i++) {
        //ret = av_frame_make_writable(avFrame_);
        for (int x = 0; x < height; x++) {
            for (int y = 0; y < width; y++) {
                yPlaneData[x * avFrame_->linesize[0] + y] = x + y + i * 3;
            }
        }

        for (int x = 0; x < height / 2; x++) {
            for (int y = 0; y < width / 2; y++) {
                uPlaneData[x * avFrame_->linesize[1] + y] = 128 + y + i * 5;
                vPlaneData[x * avFrame_->linesize[2] + y] = 64 + y + i * 8;
            }
        }
        avFrame_->pts = i;
        fwrite(yPlaneData, 1, height * avFrame_->linesize[0], file_ptr);
        fwrite(uPlaneData, 1, height / 2 * avFrame_->linesize[1], file_ptr);
        fwrite(vPlaneData, 1, height / 2 * avFrame_->linesize[2], file_ptr);
    }
    fclose(file_ptr);
    ReleaseSource();
}

