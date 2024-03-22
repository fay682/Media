#pragma once
#include"video_draw.h"

class Direct3dDraw: public VideoDraw {
  public:
    virtual void Init(int width, int height);

    virtual void Draw(AVFrame* avFrame, AVCodecContext* codec_context);

};