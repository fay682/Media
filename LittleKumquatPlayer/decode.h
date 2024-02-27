#pragma once
#include"common.h"
#include<mutex>
#include<queue>

class Decode {
  public:
    void Open(AVCodecParameters* avCodecParamters);
    bool Send(AVPacket* avPacket);
    AVFrame* Receive();

  private:
    std::mutex mutex_;
  public:
    AVCodecContext* avCodecContext_ = NULL;
};

