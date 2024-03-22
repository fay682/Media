#pragma once
#include <memory>
#include <map>
#include "common.h"
#include "video_draw.h"

class VideoDrawManager {
  private:
    VideoDrawManager();
    ~VideoDrawManager();

    void AddVideoDrawTypePtr(VideoDrawType video_draw_type, VideoDrawPtr video_draw_ptr);
    VideoDrawPtr GetVideoDrawTypePtr(VideoDrawType video_draw_type);

  public:
    static VideoDrawManager* GetInstance() {
        static VideoDrawManager* ins = new VideoDrawManager();
        return ins;
    }

  public:
    void Init(int width, int height, VideoDrawType video_draw_type);

    void Draw(AVFrame* avFrame, VideoDrawType video_draw_type, AVCodecContext* codec_context);


  private:
    int width_ = 0;
    int height_ = 0;
    std::map<VideoDrawType, VideoDrawPtr> video_draw_ptr_by_type_;
};