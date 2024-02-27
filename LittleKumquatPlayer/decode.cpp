#include "decode.h"

void Decode::Open(AVCodecParameters* avCodecParamters) {
    const AVCodec* avCodec = avcodec_find_decoder(avCodecParamters->codec_id);
    if (!avCodec) {
        std::string err_str = "没有找到相应解码器！";
        return;
    }

    //根据找到的解码器创建其上下文
    avCodecContext_ = avcodec_alloc_context3(avCodec);

    //if (avCodecParamters->format == 8) {
    //    avCodecContext_->pkt_timebase.den = 44100;
    //    avCodecContext_->pkt_timebase.num = 1;
    //} else {
    //    avCodecContext_->pkt_timebase.den = 12800;
    //    avCodecContext_->pkt_timebase.num = 1;
    //}


    //配置解码器上下文参数
    avcodec_parameters_to_context(avCodecContext_, avCodecParamters);
    //avcodec_parameters_free(&para);

    //8线程解码
    //avCodecContext_->thread_count = 8;

    int ret = 0;
    ret = avcodec_open2(avCodecContext_, avCodec, NULL);
    if (ret < 0) {
        std::string err_str = "打开解码器失败！";
        return;
    }

}

bool Decode::Send(AVPacket * avPacket) {
    mutex_.lock();
    if (!avPacket) {
        std::string err_str = "空数据包！";
        mutex_.unlock();
        return false;
    }

    //不保存数据包，直接传给解码器
    /*queue_.push(avPacket);
    mutex_.unlock();*/
    int ret = 0;
    ret = avcodec_send_packet(avCodecContext_, avPacket);
    av_packet_free(&avPacket);//释放申请的空间

    if (ret != 0) {
        std::string err_str = "数据包发送给解码器失败";
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
        std::string err_str = "获取解码数据出错";
        av_frame_free(&avFrame);
        mutex_.unlock();
        return NULL;
    }
    mutex_.unlock();
    return avFrame;
}
