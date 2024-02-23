#include "video_decode_play_thread.h"
#include <fstream>

std::string url = "test.h264";
void WriteData(AVFrame frame) {
    std::ofstream os;
    if (!os.is_open()) {
        os.open(url);
    }
    os << frame.data;
}

void VideoDecodePlayThread::Run() {
    while (!is_exist()) {
        mutex_.lock();
        AVPacket* avPacket = Pop();
        decode_->Send(avPacket);
        AVFrame* avFrame = decode_->Receive();
        while (avFrame) {
            //��Ⱦ����Ƶ����
            //��������֡
            WriteData(*avFrame);
            //�ͷ�
            av_frame_free(&avFrame);
            avFrame = decode_->Receive();
        }
        mutex_.unlock();
    }
}

void VideoDecodePlayThread::Open(AVCodecParameters* avCodecParameters) {
    mutex_.lock();
    decode_->Open(avCodecParameters);
    mutex_.unlock();
}
