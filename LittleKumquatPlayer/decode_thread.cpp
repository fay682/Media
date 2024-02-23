#include "decode_thread.h"

DecodeThread::DecodeThread() {
    decode_ = new Decode();
}

DecodeThread::~DecodeThread() {
    Wait();
}

void DecodeThread::Push(AVPacket * avPacket) {
    mutex_.lock();
    if (!avPacket || (!avPacket->data) || (avPacket->size < 0)) {
        mutex_.unlock();
        return;
    }
    queue_.push(avPacket);
    mutex_.unlock();
}

AVPacket* DecodeThread::Pop() {
    mutex_.lock();
    if (!queue_.empty()) {
        AVPacket* avPacket = queue_.front();
        queue_.pop();
        mutex_.unlock();
        return avPacket;
    }
    mutex_.unlock();
    return NULL;
}
