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

    /*GLEW����������OpenGL�ĺ���ָ��*/
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        return;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    /*����OpenGL��Ⱦ���ڵĳߴ��С*/
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

        /*��Ⱦָ��*/
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//���������Ļ���õ���ɫ��״̬��������״̬��
        glClear(GL_COLOR_BUFFER_BIT);//�����ɫ����֮��������ɫ���嶼�ᱻ����������� ��״̬Ӧ�ã�

        /* Swap front and back buffers */
        glfwSwapBuffers(window);


    }
    /*�ͷ�GLFW������ڴ�*/
    glfwTerminate();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    // ���û�����ESC��,��������window���ڵ�WindowShouldClose����Ϊtrue
    // �ر�Ӧ�ó���
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}