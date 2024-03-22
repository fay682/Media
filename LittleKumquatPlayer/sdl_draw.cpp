#pragma once
#include<Windows.h>
#include"sdl_draw.h"
#include "LittleKumquatPlayer.h"

void SdlDraw::Init(int width, int height) {
    SDL_Init(SDL_INIT_VIDEO);
    //window = SDL_CreateWindow("hello world", width, height, 0);
    window = SDL_CreateWindowFrom(GetParentWindow());
    renderer = SDL_CreateRenderer(window, NULL, 0);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, width, height);
    SDL_ShowWindow(window);
}

void SdlDraw::Draw(AVFrame* avFrame, AVCodecContext* codec_context) {

    //什么时间需要格式转换什么时候不需要
    //uint8_t* image_data_ = nullptr;
    //unsigned int image_size_ = 0;
    //auto ret = av_image_copy_to_buffer(image_data_, image_size_,
    //                                   (const uint8_t* const*)(avFrame->data),
    //                                   (const int*)avFrame->linesize, AV_PIX_FMT_NV12,
    //                                   1280, 720, 1);
    //SDL_UpdateTexture(texture, nullptr, image_data_, 1280);

    //AVPixelFormat srcFormat = codec_context->pix_fmt;
    //auto avFrame = av_frame_alloc();
    ///*align:此参数是设定内存对齐的对齐数，也就是按多大的字节进行内存对齐。比如设置为1，表示按1字节对齐，那么得到的结果就是与实际的内存大小一样。
    //再比如设置为4，表示按4字节对齐。也就是内存的起始地址必须是4的整倍数。*/
    //auto memey_size = av_image_get_buffer_size(AV_PIX_FMT_NV12, srcavFrame->width, srcavFrame->height, 0);
    //auto dst_buffer = (unsigned char*)av_malloc(memey_size);
    //int ret = av_image_fill_arrays(avFrame->data, avFrame->linesize, dst_buffer,
    //                               AV_PIX_FMT_NV12, srcavFrame->width, srcavFrame->height, 0);
    //auto sws_context = sws_getContext(srcavFrame->width, srcavFrame->height, srcFormat,
    //                                  srcavFrame->width, srcavFrame->height, AV_PIX_FMT_NV12, SWS_FAST_BILINEAR, NULL, NULL, NULL);

    //ret = sws_scale(sws_context, (const unsigned char* const*)srcavFrame->data, srcavFrame->linesize, 0, srcavFrame->height,
    //                avFrame->data, avFrame->linesize);

    SDL_UpdateYUVTexture(texture, nullptr,
                         avFrame->data[0], avFrame->linesize[0],
                         avFrame->data[1], avFrame->linesize[1],
                         avFrame->data[2], avFrame->linesize[2]);//更新纹理
    SDL_FRect sdlRect;
    sdlRect.x = 0;
    sdlRect.y = 0;
    sdlRect.w = avFrame->width;
    sdlRect.h = avFrame->height;

    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, nullptr, &sdlRect);//纹理赋值给渲染器
    SDL_RenderPresent(renderer);//显示
    SDL_Delay(40);
}