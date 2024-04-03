#include"opengl_draw.h"
#include<Windows.h>

//const GLchar* vertexShaderSource2 = "#version 330 core\n"
//                                    "layout (location = 0) in vec3 vertexIn;\n"
//                                    "layout (location = 1) in vec2 textureIn;\n"
//                                    "layout (location = 2) in vec3 matIn;\n"
//                                    "out vec2 textureOut;\n"
//                                    "out vec3 matOut;\n"
//                                    "void main()\n"
//                                    "{\n"
//                                    "gl_Position = vec4(vertexIn, 1.0);\n"
//                                    "textureOut = textureIn;\n"
//                                    "matOut = matIn;\n"
//                                    "}\0";
//
///*使用uniform*/
//const GLchar* fragmentShaderSource2 = "#version 330 core\n"
//                                      "in vec2 textureOut;\n"
//                                      "in vec3 matOut;\n"
//                                      "out vec4 color;\n"
//                                      "uniform sampler2D tex_y;\n"
//                                      "uniform sampler2D tex_u;\n"
//                                      "uniform sampler2D tex_v;\n"
//                                      "void main()\n"
//                                      "{\n"
//                                      "vec3 yuv;\n"
//                                      "vec3 rgb;\n"
//                                      "yuv.x=texture(tex_y, textureOut).r;\n"
//                                      "yuv.y=texture(tex_u, textureOut).r;\n"
//                                      "yuv.z=texture(tex_v, textureOut).r;\n"
//                                      "rgb = matOut * yuv;\n"
//                                      "color = vec4(rgb,1.0);\n"
//                                      "}\n\0";

//顶点坐标
//static const GLfloat ver[] = {
//    0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0, 1.0, 1.0,
//    0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0, -0.39465, 2.03211,
//    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.13983, -0.58060, 0.0,
//    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0, 0.0, 0.0
//};

const GLchar* vertexShaderSource2 = "#version 330 core\n"
                                    "layout (location = 0) in vec3 vertexIn;\n"
                                    "layout (location = 1) in vec2 textureIn;\n"
                                    "layout (location = 2) in vec3 matIn;\n"
                                    "out vec2 textureOut;\n"
                                    "out vec3 matOut;\n"
                                    "void main()\n"
                                    "{\n"
                                    "gl_Position = vec4(vertexIn, 1.0);\n"
                                    "textureOut = textureIn;\n"
                                    "matOut = matIn;\n"
                                    "}\0";

/*使用uniform*/
const GLchar* fragmentShaderSource2 = "#version 330 core\n"
                                      "in vec2 textureOut;\n"
                                      "in vec3 matOut;\n"
                                      "out vec4 color;\n"
                                      "uniform sampler2D tex_y;\n"
                                      "uniform sampler2D tex_u;\n"
                                      "uniform sampler2D tex_v;\n"
                                      "void main()\n"
                                      "{\n"
                                      "vec3 yuv;\n"
                                      "vec3 rgb;\n"
                                      "yuv.x=texture(tex_y, textureOut).r;\n"
                                      "yuv.y=texture(tex_u, textureOut).r;\n"
                                      "yuv.z=texture(tex_v, textureOut).r;\n"
                                      "rgb = matOut * yuv;\n"
                                      "color = vec4(rgb,1.0);\n"
                                      "}\n\0";

static const GLfloat ver[] = {
    -1.0f, -1.0f,
        1.0f, -1.0f,
        -1.0f, 1.0f,
        1.0f, 1.0f,
    };

static const GLfloat ter[] = {
    0.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f,
};

OpenglDraw::OpenglDraw() {

}

OpenglDraw::~OpenglDraw() {
    OutputDebugString(L"----------~OpenglDraw()!!!------------");
}

void OpenglDraw::Init(int width, int height) {
    width_ = width;
    height_ = height;
    delete datas[0];
    delete datas[1];
    delete datas[2];
    datas[0] = new unsigned char[width * height];//y\u\v
    datas[1] = new unsigned char[width * height / 4];
    datas[2] = new unsigned char[width * height / 4];

    if (texs[0]) {
        glDeleteTextures(3, texs);
    }

    glGenTextures(1, &texs[0]);
    glBindTexture(GL_TEXTURE_2D, texs[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenTextures(1, &texs[1]);
    glBindTexture(GL_TEXTURE_2D, texs[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width / 2, height / 2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenTextures(1, &texs[2]);
    glBindTexture(GL_TEXTURE_2D, texs[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width / 2, height / 2, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    // InitOpengl();

}

void OpenglDraw::InitOpengl() {
    if (!glfwInit())
        return;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(width_, height_, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        return;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    //编译shader
    //创建一个shader对象
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //将源码附加到对象上
    glShaderSource(vertexShader, 1, &vertexShaderSource2, NULL);
    glCompileShader(vertexShader);
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        OutputDebugString(L"----------myShader error!!!------------");
    }

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource2, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        OutputDebugString(L"----------myShader error!!!------------");
    }

    //shader程序
    //创建一个程序对象
    //GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    //编译的shader附加到程序对象
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        OutputDebugString(L"ERROR::SHADER::VERTEX::COMPILATION_FAILED");
    }

    // 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLuint VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ver), ver, GL_DYNAMIC_DRAW);

    //解析顶点数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    unis[0] = glGetUniformLocation(shaderProgram, "tex_y");
    unis[1] = glGetUniformLocation(shaderProgram, "tex_u");
    unis[2] = glGetUniformLocation(shaderProgram, "tex_v");
}

bool flag = true;
void OpenglDraw::Draw(AVFrame* srcavFrame, AVCodecContext* codec_context) {
    if (flag) {
        InitOpengl();
        flag = false;
    }
    if (!srcavFrame) {
        return;
    }
    if (width_ != srcavFrame->width || height_ != srcavFrame->height) {
        return;
    }

    AVPixelFormat srcFormat = codec_context->pix_fmt;
    auto avFrame = av_frame_alloc();
    /*align:此参数是设定内存对齐的对齐数，也就是按多大的字节进行内存对齐。比如设置为1，表示按1字节对齐，那么得到的结果就是与实际的内存大小一样。
    再比如设置为4，表示按4字节对齐。也就是内存的起始地址必须是4的整倍数。*/
    auto memey_size = av_image_get_buffer_size(AV_PIX_FMT_YUV420P, srcavFrame->width, srcavFrame->height, 1);
    auto dst_buffer = (unsigned char*)av_malloc(memey_size);
    av_image_fill_arrays(avFrame->data, avFrame->linesize, dst_buffer,
                         AV_PIX_FMT_YUV420P, srcavFrame->width, srcavFrame->height, 1);
    auto sws_context = sws_getContext(srcavFrame->width, srcavFrame->height, srcFormat,
                                      srcavFrame->width, srcavFrame->height, AV_PIX_FMT_YUV420P, SWS_FAST_BILINEAR, NULL, NULL, NULL);

    sws_scale(sws_context, (const unsigned char* const*)srcavFrame->data, srcavFrame->linesize, 0, srcavFrame->height,
              avFrame->data, avFrame->linesize);


    if (width_ == avFrame->linesize[0]) {
        OutputDebugString(L"testLog:");
        memcpy(datas[0], avFrame->data[0], width_ * height_);
        memcpy(datas[1], avFrame->data[1], width_ * height_ / 4);
        memcpy(datas[2], avFrame->data[2], width_ * height_ / 4);
    } else {
        for (int i = 0; i < height_; i++)
            memcpy(datas[0] + width_ * i, avFrame->data[0] + avFrame->linesize[0] * i, width_);
        for (int i = 0; i < height_ / 2; i++)
            memcpy(datas[1] + width_ / 2 * i, avFrame->data[1] + avFrame->linesize[1] * i, width_);
        for (int i = 0; i < height_ / 2; i++)
            memcpy(datas[2] + width_ / 2 * i, avFrame->data[2] + avFrame->linesize[2] * i, width_);
    }

    if (!glfwWindowShouldClose(window)) {

        glfwPollEvents();
        // 清空颜色缓冲
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // 记得激活着色器
        glUseProgram(shaderProgram);

        // 更新uniform颜色
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texs[0]);
        glTexSubImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 0, width_, height_, GL_LUMINANCE, GL_UNSIGNED_BYTE, datas[0]);
        glUniform1f(unis[0], 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texs[1]);
        glTexSubImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 0, width_ / 2, height_ / 2, GL_LUMINANCE, GL_UNSIGNED_BYTE, datas[1]);
        glUniform1f(unis[1], 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texs[2]);
        glTexSubImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, 0, width_ / 2, height_ / 2, GL_LUMINANCE, GL_UNSIGNED_BYTE, datas[2]);
        glUniform1f(unis[2], 2);

        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 5);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);
        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }
}
