#pragma once
#include<memory>
#include"common.h"

enum class VideoDrawType {
    kSDL = 0,
    kOpengl,
    kD3D
};

class VideoDraw {
  public:
    virtual void Init(int width, int height) = 0;
    virtual void Draw(AVFrame* avFrame, AVCodecContext* codec_context) = 0;
};

using VideoDrawPtr = std::shared_ptr<VideoDraw>;