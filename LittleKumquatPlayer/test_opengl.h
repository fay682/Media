#pragma once
#include "GL/glew.h"
#include"GLFW/glfw3.h"
#pragma comment(lib,"lib-vc2022\\glfw3.lib")
#pragma comment(lib,"opengl32.lib")

#ifdef _DEBUG
#pragma comment(lib,"glew32d.lib")
#else
#pragma comment(lib,"glew32.lib")
#endif // DEBUG



void DrawAngles();

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void LogState(GLuint myShader);
