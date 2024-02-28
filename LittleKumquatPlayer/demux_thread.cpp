#include "demux_thread.h"
#include<Windows.h>

DemuxThread::DemuxThread(): demux_(NULL) {
}

DemuxThread::~DemuxThread() {
    Wait();//���װ�߳�һֱ���У����������������������������
}

void DemuxThread::Open(std::string url) {
    mutex_.lock();
    if (!demux_) {
        demux_ = new Demux();
    }
    if (!video_decode_play_thread_) {
        video_decode_play_thread_ = new VideoDecodePlayThread();
    }
    demux_->Open(url);
    video_decode_play_thread_->Open(demux_->CopyVideoParam());
    mutex_.unlock();
}

void DemuxThread::StartRelate() {
    //�������װ�̡߳����벥���߳�
    mutex_.lock();
    if (!demux_) {
        demux_ = new Demux();
    }
    if (!video_decode_play_thread_) {
        video_decode_play_thread_ = new VideoDecodePlayThread();
    }
    Start();
    video_decode_play_thread_->Start();
    mutex_.unlock();
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

void DemuxThread::Close() {
    Wait();
    if (video_decode_play_thread_) {
        video_decode_play_thread_->Close();
    }
    mutex_.lock();
    delete video_decode_play_thread_;
    video_decode_play_thread_ = NULL;
    mutex_.unlock();
}