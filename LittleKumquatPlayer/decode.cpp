#include "decode.h"

void Decode::Open(AVCodecParameters* avCodecParamters) {
    const AVCodec* avCodec = avcodec_find_decoder(avCodecParamters->codec_id);
    if (!avCodec) {
        std::string err_str = "û���ҵ���Ӧ��������";
        return;
    }

    //�����ҵ��Ľ�����������������
    avCodecContext_ = avcodec_alloc_context3(avCodec);

    //if (avCodecParamters->format == 8) {
    //    avCodecContext_->pkt_timebase.den = 44100;
    //    avCodecContext_->pkt_timebase.num = 1;
    //} else {
    //    avCodecContext_->pkt_timebase.den = 12800;
    //    avCodecContext_->pkt_timebase.num = 1;
    //}


    //���ý����������Ĳ���
    avcodec_parameters_to_context(avCodecContext_, avCodecParamters);
    //avcodec_parameters_free(&para);

    //8�߳̽���
    //avCodecContext_->thread_count = 8;

    int ret = 0;
    ret = avcodec_open2(avCodecContext_, avCodec, NULL);
    if (ret < 0) {
        std::string err_str = "�򿪽�����ʧ�ܣ�";
        return;
    }

}

bool Decode::Send(AVPacket * avPacket) {
    mutex_.lock();
    if (!avPacket) {
        std::string err_str = "�����ݰ���";
        mutex_.unlock();
        return false;
    }

    //���������ݰ���ֱ�Ӵ���������
    /*queue_.push(avPacket);
    mutex_.unlock();*/
    int ret = 0;
    ret = avcodec_send_packet(avCodecContext_, avPacket);
    av_packet_free(&avPacket);//�ͷ�����Ŀռ�

    if (ret != 0) {
        std::string err_str = "���ݰ����͸�������ʧ��";
        mutex_.unlock();
        return false;
    }
    mutex_.unlock();
    return true;
}

AVFrame* Decode::Receive() {
    mutex_.lock();
    AVFrame* avFrame = av_frame_alloc();
    int ret = 0;
    ret = avcodec_receive_frame(avCodecContext_, avFrame);
    if (ret != 0) {
        std::string err_str = "��ȡ�������ݳ���";
        av_frame_free(&avFrame);
        mutex_.unlock();
        return NULL;
    }
    mutex_.unlock();
    return avFrame;
}
