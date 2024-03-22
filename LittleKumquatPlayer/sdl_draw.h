#pragma once
#include "SDL3/SDL.h"
#include "common.h"
#include "video_draw.h"

#pragma comment(lib,"VisualC/x64/Release/SDL3.lib")

class SdlDraw: public VideoDraw {
  public:

    virtual void Init(int width, int height);

    virtual void Draw(AVFrame* avFrame, AVCodecContext* codec_context);

  private:
    SDL_Window* window = NULL;//ǽ
    SDL_Renderer* renderer = NULL;//����
    SDL_Texture* texture = NULL;//����
    SDL_Surface* surface = NULL;//��ֽ
};