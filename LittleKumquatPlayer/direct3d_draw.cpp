#include <d3d11.h>
#include "direct3d_draw.h"

void Direct3dDraw::Init(int width, int height) {

}

void Direct3dDraw::Draw(AVFrame* avFrame, AVCodecContext* codec_context) {
    ID3D11Texture2D* texture = (ID3D11Texture2D*)avFrame->data[0];
}