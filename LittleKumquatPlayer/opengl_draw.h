#pragma once
#include<memory>
#include"video_draw.h"
#include"common.h"
#include "GL/glew.h"
#include"GLFW/glfw3.h"
#pragma comment(lib,"lib-vc2022\\glfw3.lib")
#pragma comment(lib,"opengl32.lib")
#ifdef _DEBUG
#pragma comment(lib,"glew32d.lib")
#else
#pragma comment(lib,"glew32.lib")
#endif // DEBUG


class OpenglDraw: public VideoDraw {
  public:
    OpenglDraw();
    ~OpenglDraw();

    virtual void Init(int width, int height);

    virtual void Draw(AVFrame* avFrame, AVCodecContext* codec_context);

    void InitOpengl();



  private:
    //²ÄÖÊÄÚ´æ
    unsigned char* datas[3] = { 0 };
    //textureµØÖ·
    GLuint texs[3] = { 0 };
    //yuv
    GLuint unis[3] = { 0 };

    GLFWwindow* window;
    int width_ = 0;
    int height_ = 0;

    GLuint shaderProgram;
    GLuint VAO;
};
