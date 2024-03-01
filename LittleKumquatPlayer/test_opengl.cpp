#include"test_opengl.h"

void DrawAngles() {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(480, 320, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /*GLEW是用来管理OpenGL的函数指针*/
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        return;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    /*告诉OpenGL渲染窗口的尺寸大小*/
    glViewport(1000, 500, width, height);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        ///* Draw a triangle */
        //glBegin(GL_TRIANGLES);

        //glColor3f(1.0, 0.0, 0.0);    // Red
        //glVertex3f(0.0, 1.0, 0.0);

        //glColor3f(0.0, 1.0, 0.0);    // Green
        //glVertex3f(-1.0, -1.0, 0.0);

        //glColor3f(0.0, 0.0, 1.0);    // Blue
        //glVertex3f(1.0, -1.0, 0.0);

        //glEnd();

        /* Poll for and process events */
        glfwPollEvents();

        /*渲染指令*/
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//设置清空屏幕所用的颜色（状态机：设置状态）
        glClear(GL_COLOR_BUFFER_BIT);//清除颜色缓冲之后，整个颜色缓冲都会被上面设置填充 （状态应用）

        /* Swap front and back buffers */
        glfwSwapBuffers(window);


    }
    /*释放GLFW分配的内存*/
    glfwTerminate();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    // 当用户按下ESC键,我们设置window窗口的WindowShouldClose属性为true
    // 关闭应用程序
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}