#include "demux_thread.h"
#include<Windows.h>

DemuxThread::DemuxThread(): demux_(NULL) {
}

DemuxThread::~DemuxThread() {
    Wait();//���װ�߳�һֱ���У����������������������������
}

void DemuxThread::Open(std::string url) {
    if (!demux_) {
        demux_ = new Demux();
    }
    demux_->Open(url);
    if (!video_decode_play_thread_) {
        video_decode_play_thread_ = new VideoDecodePlayThread();
    }
    video_decode_play_thread_->Open(demux_->CopyVideoParam());
}

void DemuxThread::StartRelate() {
    //�������װ�̡߳����벥���߳�
    if (!video_decode_play_thread_) {
        video_decode_play_thread_ = new VideoDecodePlayThread();
    }
    video_decode_play_thread_->Start();
}


void DemuxThread::Run() {
    while (!is_exist()) {
        mutex_.lock();
        if (!demux_) {
            mutex_.unlock();
            Sleep(5);
            continue;
        }
        AVPacket* pkt = demux_->Read();
        if (!pkt) {
            mutex_.unlock();
            Sleep(5);
            continue;
        }
        //������Ӧ������
        if (demux_->IsAuidoPacket(pkt)) {

        } else {
            video_decode_play_thread_->Push(pkt);
        }
        mutex_.unlock();
        Sleep(1);
    }
}
