#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640

typedef struct StaticDrawData {
    GLuint program;
    GLuint vao;
    GLuint ebo;
} StaticDrawData;

const GLchar* const vertexShaderSource =
"#version 300 es\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.f);\n"
"}";

const GLchar* const fragmentShaderSource =
"#version 300 es\n"
"precision lowp float;\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main() {\n"
"   FragColor = ourColor;\n"
"}";

unsigned int drawModeIndex = 0;
const GLenum drawModes[] = {
    GL_TRIANGLES,
    GL_LINES,
    GL_LINE_STRIP,
    GL_LINE_LOOP,
    GL_POINTS
};

void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    } else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        if (drawModeIndex >= (sizeof(drawModes) / sizeof(drawModes[0]) - 1)) {
            drawModeIndex = 0;
        } else {
            drawModeIndex++;
        }
    }
}

void setGlfwWindowHints(void) {
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_FOCUSED, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
}

GLint checkShaderCompileError(GLuint shader) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar* infoLog = (char*) malloc(infoLogLength * sizeof(char));
        glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);
        fprintf(stderr, "Error: Shader Compilation failed: %s", infoLog);
        free(infoLog);
    }
    return success;
}

GLint checkProgramLinkError(GLuint program) {
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLint infoLogLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
        GLchar* infoLog = (char*) malloc(infoLogLength * sizeof(char));
        glGetProgramInfoLog(program, infoLogLength, NULL, infoLog);
        fprintf(stderr, "Error: Linking Program failed: %s", infoLog);
        free(infoLog);
    }
    return success;
}

void showGlInfo(void) {
    GLint nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    printf("Maximum number of vertex attributes: %d\n", nrAttributes);
}

void update(double delta) {
    // Logic
}

void draw(double delta, StaticDrawData staticData) {
    glClearColor(0.18f, 0.20f, 0.21f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(staticData.program);
    float greenValue = (sin(glfwGetTime()) / 2.f) + 0.5f;
    GLint ourColorLoc = glGetUniformLocation(staticData.program, "ourColor");
    glUniform4f(ourColorLoc, 0.f, greenValue, 0.f, 1.f);
    glBindVertexArray(staticData.vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, staticData.ebo);
    glDrawElements(drawModes[drawModeIndex], 6, GL_UNSIGNED_INT, 0);
}

int main(void) {
    printf("GLFW version: %s\n",  glfwGetVersionString());
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        return EXIT_FAILURE;
    }
    setGlfwWindowHints();
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Learn OpenGL <learnopengl.com>", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLES2Loader((GLADloadproc) glfwGetProcAddress)) {
        fputs("Error: Failed to initialize GLAD\n", stderr);
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwSetKeyCallback(window, key_callback);
    showGlInfo();
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    checkShaderCompileError(vertexShader);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    checkShaderCompileError(fragmentShader);
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkProgramLinkError(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLfloat vertices[] = {
        0.5f,  0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };
    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    GLuint virtArrObj, virtBuffObj, elemBuffObj;
    glGenVertexArrays(1, &virtArrObj);
    glGenBuffers(1, &virtBuffObj);
    glGenBuffers(1, &elemBuffObj);
    glBindVertexArray(virtArrObj);
    glBindBuffer(GL_ARRAY_BUFFER, virtBuffObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elemBuffObj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    StaticDrawData staticDrawData = {
        .program = shaderProgram,
        .vao = virtArrObj,
        .ebo = elemBuffObj
    };

    double timeFreq = (double) glfwGetTimerFrequency();
    uint64_t timeLast = glfwGetTimerValue();
    uint64_t timeNow;
    double timeDelta;
    while (!glfwWindowShouldClose(window)) {
        timeNow = glfwGetTimerValue();
        timeDelta = (timeNow - timeLast) / timeFreq;
        timeLast = timeNow;
        update(timeDelta);
        draw(timeDelta, staticDrawData);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return EXIT_SUCCESS;
}