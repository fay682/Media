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

/*使用uniform*/
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

/*三角形*/
GLfloat vertices[] = {
    0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
    0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
};

/*矩形*/
//GLfloat vertices[] = {
//    0.5f, 0.5f, 0.0f,   // 右上角
//    0.5f, -0.5f, 0.0f,  // 右下角
//    -0.5f, -0.5f, 0.0f, // 左下角
//    -0.5f, 0.5f, 0.0f   // 左上角
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

    /*GLEW是用来管理OpenGL的函数指针*/
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        return;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    /*告诉OpenGL渲染窗口的尺寸大小*/
    glViewport(0, 0, width, height);

    //编译shader
    //创建一个shader对象
    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //将源码附加到对象上
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    LogState(vertexShader);

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    LogState(fragmentShader);

    //shader程序
    //创建一个程序对象
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    //编译的shader附加到程序对象
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

    // 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
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
    glBindBuffer(GL_ARRAY_BUFFER, VBO);//对GL_ARRAY_BUFFER的调用就是调用绑定的VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //解析顶点数据
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    //glEnableVertexAttribArray(0);

    //解析带颜色的顶点数据
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

        ///*渲染指令*/
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//设置清空屏幕所用的颜色（状态机：设置状态）
        glClear(GL_COLOR_BUFFER_BIT);//清除颜色缓冲之后，整个颜色缓冲都会被上面设置填充 （状态应用）

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
    /*释放GLFW分配的内存*/
    glfwTerminate();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    // 当用户按下ESC键,我们设置window窗口的WindowShouldClose属性为true
    // 关闭应用程序
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

