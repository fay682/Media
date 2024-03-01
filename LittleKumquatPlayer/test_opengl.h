#pragma once
#include "GL/glew.h"
#include"GLFW/glfw3.h"
#pragma comment(lib,"lib-vc2022\\glfw3.lib")
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glew32.lib")

void DrawAngles();

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
