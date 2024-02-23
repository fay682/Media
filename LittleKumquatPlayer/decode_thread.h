#pragma once
#include"x_thread.h"
#include "decode.h"
#include"common.h"
#include<mutex>
#include<queue>

class DecodeThread: public XThread {
  public:
    DecodeThread();
    ~DecodeThread();

    void Push(AVPacket* avPacket);
    AVPacket* Pop();

  private:
    std::mutex mutex_;
    std::queue<AVPacket*> queue_;

  protected:
    Decode* decode_ = NULL;
};

