#pragma once
#include "x_thread.h"
#include<string>
#include"demux.h"
#include<mutex>
#include"video_decode_Play_Thread.h"

class DemuxThread: public XThread {
  public:

    DemuxThread();
    ~DemuxThread();

    void Open(std::string url);

    void StartRelate();

    virtual void Run() override;

  private:
    Demux* demux_;
    std::mutex mutex_;
    VideoDecodePlayThread* video_decode_play_thread_;

};

