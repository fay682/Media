#pragma once
#include<string>
extern "C" {
#include "libavformat/avformat.h"
#include "libavutil/log.h"
}

class MediaSetting {
  public:
    void SetFfmpegLog();

    void ReleaseSource();


    void GetAudioDataFromMediaFile(std::string const& src, std::string const& dst);

    void GetVideoDataFromMediaFile(std::string const& src, std::string const& dst);
    //ý���ļ���ʽת�� MP4->mov
    void RemuxMediaFile(std::string const& src, std::string const& dst);
    //ý���ļ��ü�
    void CutMediaFile(const std::string& src, const std::string& dst, double start_time, double end_time);
    //����һ��yuv�ļ�
    void CreateYuvVideoFile(const std::string& src);

  private:
    int stream_index_ = -1;
    AVFormatContext* avFormatContextInput_ = nullptr;
    AVFormatContext* avFormatContextOutput_ = nullptr;
    AVStream* avStream_ = nullptr;
    AVCodecParameters* avCodecParametersInput_ = nullptr;
    AVCodecParameters* avCodecParametersOutput_ = nullptr;
    AVPacket avPacket_;
    int* avStream_index_ = nullptr;
    int* avStream_seek_pts_ = nullptr;
    int* avStream_seek_dts_ = nullptr;
    AVFrame* avFrame_ = nullptr;
};