#pragma once
#include<string>
#include"common.h"

class Demux {
  public:
    void Open(std::string url);

    AVPacket* Read();
    bool IsAuidoPacket(AVPacket* avPacket);

    AVCodecParameters* CopyVideoParam();
    AVCodecParameters* CopyAudioParam();

  private:
    AVFormatContext* avFormatCtxPtr_ = NULL;
    int audio_stream_;
    int video_stream_;

    int width_;
    int heigh_;
    int sample_rate_;
    int channels_;
    int video_format_;
    int audio_format_;

};

