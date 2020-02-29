#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>

#include "glad/glad.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "shader.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640

typedef struct StaticDrawData {
    GLuint program;
    GLuint vao;
    GLuint ebo;
} StaticDrawData;

unsigned int drawModeIndex = 0;
const GLenum drawModes[] = {
    GL_TRIANGLES,
    GL_LINES,
    GL_LINE_STRIP,
    GL_LINE_LOOP,
    GL_POINTS
};

void errorCallback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
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
    glfwSetErrorCallback(errorCallback);
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
    glfwSetKeyCallback(window, keyCallback);
    showGlInfo();
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    GLuint shaderProgram = createShaderProgram("resources/vertex.glsl", "resources/fragment.glsl");
    GLfloat vertices[] = {
         1.f, -0.8f, 0.f, 1.f, 0.f, 0.f,
        -1.f, -0.8f, 0.f, 0.f, 1.f, 0.f,
         0.f,  0.8f, 0.f, 0.f, 0.f, 1.f
    };
    GLuint indices[] = {
        0, 1, 2
    };
    GLuint virtArrObj, virtBuffObj, elemBuffObj;
    glGenVertexArrays(1, &virtArrObj);
    glGenBuffers(1, &virtBuffObj);
    glGenBuffers(1, &elemBuffObj);
    glBindVertexArray(virtArrObj);
    glBindBuffer(GL_ARRAY_BUFFER, virtBuffObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
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