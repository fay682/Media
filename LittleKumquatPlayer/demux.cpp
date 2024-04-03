#include "demux.h"
#include"video_draw_manager.h"

void Demux::Open(std::string url) {
    //���װ��ȡ����
    avFormatCtxPtr_ = avformat_alloc_context();
    int ret = 0;
    ret = avformat_open_input(&avFormatCtxPtr_, url.c_str(), NULL, NULL);
    if (ret != 0) {
        std::string err_str = "����������ͷ�ļ���Ϣ��";
        return;
    }
    video_stream_ = av_find_best_stream(avFormatCtxPtr_, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if (video_stream_ < 0) {
        std::string err_str = "��ȡ��Ƶ������";
        return;
    }
    audio_stream_ = av_find_best_stream(avFormatCtxPtr_, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (video_stream_ < 0) {
        std::string err_str = "��ȡ��Ƶ������";
        return;
    }

    for (int i = 0; i < avFormatCtxPtr_->nb_streams; i++) {
        AVStream* avStreamPtr = avFormatCtxPtr_->streams[i];
        AVCodecParameters* avCodecParam = avStreamPtr->codecpar;
        if (avCodecParam->codec_type == AVMEDIA_TYPE_AUDIO) {
            //channels_ = avCodecParam->channels;
            sample_rate_ = avCodecParam->sample_rate;
            audio_format_ = avCodecParam->format;
        } else if (avCodecParam->codec_type == AVMEDIA_TYPE_VIDEO) {
            width_ = avCodecParam->width;
            heigh_ = avCodecParam->height;
            video_format_ = avCodecParam->format;
            VideoDrawManager::GetInstance()->Init(width_, heigh_, VideoDrawType::kOpengl);
        }
    }


    //audio��channel sampleRate

    //video: width heigh  fps

}
static double r2d(AVRational r) {
    return r.den == 0 ? 0 : (double)r.num / (double)r.den;
}
AVPacket* Demux::Read() {
    //���ϻ�ȡ���ݰ�--���װ�߳�(DemuxThread::Run())
    AVPacket* avPacketPtr = av_packet_alloc();
    int ret = 0;
    ret = av_read_frame(avFormatCtxPtr_, avPacketPtr);
    if (ret != 0) {
        std::string err_str = "��ȡ���ݰ�����";
        return NULL;
    }
    avPacketPtr->pts = avPacketPtr->pts * (1000 * (r2d(avFormatCtxPtr_->streams[avPacketPtr->stream_index]->time_base)));
    avPacketPtr->dts = avPacketPtr->dts * (1000 * (r2d(avFormatCtxPtr_->streams[avPacketPtr->stream_index]->time_base)));
    return avPacketPtr;
}

bool Demux::IsAuidoPacket(AVPacket* avPacket) {
    if (avPacket->stream_index == video_stream_) {
        return false;
    }
    return true;
}

AVCodecParameters* Demux::CopyVideoParam() {
    AVCodecParameters* avCodecParam = avcodec_parameters_alloc();
    int ret = 0;
    ret = avcodec_parameters_copy(avCodecParam, avFormatCtxPtr_->streams[video_stream_]->codecpar);
    if (ret < 0) {
        std::string err_str = "��ȡ��Ƶ��������������";
        return NULL;
    }
    return avCodecParam;
}

AVCodecParameters* Demux::CopyAudioParam() {
    AVCodecParameters* avCodecParam = avcodec_parameters_alloc();
    int ret = 0;
    ret = avcodec_parameters_copy(avCodecParam, avFormatCtxPtr_->streams[audio_stream_]->codecpar);
    if (ret < 0) {
        std::string err_str = "��ȡ��Ƶ��������������";
        return NULL;
    }
    return avCodecParam;
}

