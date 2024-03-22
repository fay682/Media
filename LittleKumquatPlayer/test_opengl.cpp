#include"test_opengl.h"
#include<Windows.h>
#include<math.h>

const GLchar* vertexShaderSource = "#version 330 core\n"
                                   "layout (location = 0) in vec3 position;\n"
                                   "layout (location = 1) in vec3 color;\n"
                                   "out vec3 ourColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "gl_Position = vec4(position, 1.0);\n"
                                   "ourColor=color;\n"
                                   "}\0";

/*ʹ��uniform*/
//const GLchar* fragmentShaderSource = "#version 330 core\n"
//                                     "out vec4 color;\n"
//                                     "uniform vec4 ourColor;\n"
//                                     "void main()\n"
//                                     "{\n"
//                                     "color = ourColor;\n"
//                                     "}\n\0";

const GLchar* fragmentShaderSource = "#version 330 core\n"
                                     "in vec3 ourColor;\n"
                                     "out vec4 color;\n"
                                     "void main()\n"
                                     "{\n"
                                     "color = vec4(ourColor,1.0f);\n"
                                     "}\n\0";

/*������*/
GLfloat vertices[] = {
    0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // ����
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // ����
    0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // ����
};

/*����*/
//GLfloat vertices[] = {
//    0.5f, 0.5f, 0.0f,   // ���Ͻ�
//    0.5f, -0.5f, 0.0f,  // ���½�
//    -0.5f, -0.5f, 0.0f, // ���½�
//    -0.5f, 0.5f, 0.0f   // ���Ͻ�
//};
//
//GLuint indices[] = {
//    0, 1, 3,
//    1, 2, 3
//};

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
    glViewport(0, 0, width, height);

    //����shader
    //����һ��shader����
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //��Դ�븽�ӵ�������
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    LogState(vertexShader);

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    LogState(fragmentShader);

    //shader����
    //����һ���������
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    //�����shader���ӵ��������
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        OutputDebugString( L"ERROR::SHADER::VERTEX::COMPILATION_FAILED");
    }

    // ɾ����ɫ���������Ѿ����ӵ����ǵĳ������ˣ��Ѿ�������Ҫ��
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    //glUseProgram(shaderProgram);


    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);

    //GLuint EBO;
    //glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);//��GL_ARRAY_BUFFER�ĵ��þ��ǵ��ð󶨵�VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //������������
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    //glEnableVertexAttribArray(0);

    //��������ɫ�Ķ�������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER,
                 0);//// Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    glBindVertexArray(0);


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

        ///*��Ⱦָ��*/
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//���������Ļ���õ���ɫ��״̬��������״̬��
        glClear(GL_COLOR_BUFFER_BIT);//�����ɫ����֮��������ɫ���嶼�ᱻ����������� ��״̬Ӧ�ã�

        glUseProgram(shaderProgram);

        GLfloat timeValue = glfwGetTime();
        GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
        GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 2.0f, 1.0f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
    /*�ͷ�GLFW������ڴ�*/
    glfwTerminate();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    // ���û�����ESC��,��������window���ڵ�WindowShouldClose����Ϊtrue
    // �ر�Ӧ�ó���
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}


void LogState(GLuint myShader) {
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(myShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(myShader, 512, NULL, infoLog);
        OutputDebugString(L"----------myShader error!!!------------");
    }
}

