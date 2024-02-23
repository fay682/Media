#pragma once
#include"common.h"
#include<mutex>
#include<queue>

class Decode {
  public:
    void Open(AVCodecParameters* avCodecParamters);
    void Send(AVPacket* avPacket);
    AVFrame* Receive();

  private:
    std::mutex mutex_;
    AVCodecContext* avCodecContext_ = NULL;
};

