#include"video_draw_manager.h"
#include"opengl_draw.h"
#include"sdl_draw.h"
#include"direct3d_draw.h"

VideoDrawManager::VideoDrawManager() {

}
VideoDrawManager::~VideoDrawManager() {

}

void VideoDrawManager::AddVideoDrawTypePtr(VideoDrawType video_draw_type, VideoDrawPtr video_draw_ptr) {
    video_draw_ptr_by_type_[video_draw_type] = video_draw_ptr;
}

VideoDrawPtr VideoDrawManager::GetVideoDrawTypePtr(VideoDrawType video_draw_type) {
    return video_draw_ptr_by_type_[video_draw_type];
}

void VideoDrawManager::Init(int width, int height, VideoDrawType video_draw_type) {
    switch (video_draw_type) {
    case VideoDrawType::kSDL:
        AddVideoDrawTypePtr(video_draw_type, std::make_shared<SdlDraw>());
        break;
    case VideoDrawType::kOpengl:
        AddVideoDrawTypePtr(video_draw_type, std::make_shared<OpenglDraw>());
        break;
    case VideoDrawType::kD3D:
        AddVideoDrawTypePtr(video_draw_type, std::make_shared<Direct3dDraw>());
        break;
    default:
        break;
    }

    auto video_draw_ptr = GetVideoDrawTypePtr(video_draw_type);
    video_draw_ptr->Init(width, height);
}

void VideoDrawManager::Draw(AVFrame* avFrame, VideoDrawType video_draw_type, AVCodecContext* codec_context) {
    auto video_draw_ptr = GetVideoDrawTypePtr(video_draw_type);
    video_draw_ptr->Draw(avFrame, codec_context);
}

