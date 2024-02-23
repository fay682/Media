#pragma once
#include"decode_thread.h"
#include<mutex>

class VideoDecodePlayThread : public DecodeThread {
  public:
    virtual void Run() override;

    void Open(AVCodecParameters* avCodecParameters);
  private:
    std::mutex mutex_;
};

