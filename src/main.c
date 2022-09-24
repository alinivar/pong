
#include <assert.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "lmath.h"

typedef struct Vertex_s {
    float position[3];
} Vertex_t;

typedef struct Rect_s {
    float offset[2];
    float extent[2];
} Rect_t;

static mat4 transform = {0};

static unsigned int vao, vbo, ebo;
static unsigned int vsh, fsh, pipeline;

/* player movement stuff */
static const float playerMoveSpeed = 50.0f;

static float player1Dp, player1DDp;
static float player2Dp, player2DDp;

/* ball movement stuff */
static float ballDX = -0.7f, ballDY = 0.0f;

/* rectangles */
static Rect_t player1 = {{-0.95f, 0.0f}, {0.04f, 0.65f}};
static Rect_t player2 = {{ 0.95f, 0.0f}, {0.04f, 0.65f}};

static Rect_t ball = {{0.0f, 0.0f}, {0.02f, 0.04f}};

static void resizeViewport(GLFWwindow* win, int width, int height) {
    (void) win;

    glViewport(0, 0, width, height);
    glScissor(0, 0, width, height);
}

static char* loadASCIIFile(const char* path, size_t* outSize) {
    size_t len = {0};
    char* buf = {0};
    FILE* file = {0};

    fopen_s(&file, path, "rb");
    assert(file);

    fseek(file, 0, SEEK_END);
    len = _ftelli64(file);
    fseek(file, 0, SEEK_SET);

    buf = calloc(len+1, sizeof(char));
    assert(buf);

    fread(buf, len, 1, file);
    fclose(file);

    if (outSize)
        *outSize = len;

    return buf;
}

static void processInput(GLFWwindow* win) {
    // escape
    if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(win, GLFW_TRUE);
    }

    // player 1 input
    player1DDp = 0.0f;
    if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) {
        player1DDp += playerMoveSpeed;
    }
    if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) {
        player1DDp -= playerMoveSpeed;
    }
    
    // player 2 input
    player2DDp = 0.0f;
    if (glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS) {
        player2DDp += playerMoveSpeed;
    }
    if (glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS) {
        player2DDp -= playerMoveSpeed;
    }
}

static void simulatePhysics(float delta) {
    // player 1 physics
    player1DDp -= player1Dp * 10.0f;

    player1.offset[1] = player1.offset[1] + player1Dp * delta + player1DDp * delta * delta * 0.5f;
    player1Dp = player1Dp + player1DDp * delta;
    
    // player 2 physics
    player2DDp -= player2Dp * 5.0f;

    player2.offset[1] = player2.offset[1] + player2Dp * delta + player2DDp * delta * delta * 0.5f;
    player2Dp = player2Dp + player2DDp * delta;

    // ball physics
    ball.offset[0] += ballDX * delta;
    ball.offset[1] += ballDY * delta;

    // player1 && ball collision
    if (ball.offset[0] + (ball.extent[0] / 2.0f) < player1.offset[0] + (player1.extent[0] / 2.0f) &&
        ball.offset[0] - (ball.extent[0] / 2.0f) > player1.offset[0] - (player1.extent[0] / 2.0f) &&
        ball.offset[1] + (ball.extent[1] / 2.0f) < player1.offset[1] + (player1.extent[1] / 2.0f) &&
        ball.offset[1] + (ball.extent[1] / 2.0f) > player1.offset[1] - (player1.extent[1] / 2.0f)) {
        ball.offset[0] = player1.offset[0] + (player1.extent[0] / 2.0f);
        ballDX *= -1.01f;
        ballDY = (ball.offset[1] - player1.offset[1]) * 2.0f + player1Dp * 0.75f;
    }

    // player2 && ball collision
    if (ball.offset[0] + (ball.extent[0] / 2.0f) < player2.offset[0] + (player2.extent[0] / 2.0f) &&
        ball.offset[0] - (ball.extent[0] / 2.0f) > player2.offset[0] - (player2.extent[0] / 2.0f) &&
        ball.offset[1] + (ball.extent[1] / 2.0f) < player2.offset[1] + (player2.extent[1] / 2.0f) &&
        ball.offset[1] + (ball.extent[1] / 2.0f) > player2.offset[1] - (player2.extent[1] / 2.0f)) {
        ball.offset[0] = player2.offset[0] - (player2.extent[0] / 2.0f);
        ballDX *= -1.01f;
        ballDY = (ball.offset[1] - player2.offset[1]) * 2.0f + player2Dp * 0.75f;
    }

    // ball && arena collision
    if (ball.offset[1] + (ball.extent[1] / 2.0f) > 1.0f) {
        ball.offset[1] = 1.0f - (ball.extent[1] / 2.0f);
        ballDY *= -1.0f;
    }
    if (ball.offset[1] - (ball.extent[1] / 2.0f) < -1.0f) {
        ball.offset[1] = -1.0f + (ball.extent[1] / 2.0f);
        ballDY *= -1.0f;
    }

    // lose condition

    // player1 lose
    if (ball.offset[0] - (ball.extent[0] / 2.0f) < -1.0f) {
        ball = (Rect_t){{0.0f, 0.0f}, {0.02f, 0.04f}};
        ballDX = -1.0f;
        ballDY = 0.0f;
    }
    // player2 lose
    if (ball.offset[0] + (ball.extent[0] / 2.0f) > 1.0f) {
        ball = (Rect_t){{0.0f, 0.0f}, {0.02f, 0.04f}};
        ballDX = -1.0f;
        ballDY = 0.0f;
    }

    // player1 && arena collision
    if (player1.offset[1] + (player1.extent[1] / 2.0f) > 1.0f) {
        player1.offset[1] = 1.0f - (player1.extent[1] / 2.0f);
        player1Dp = 0;
    }
    if (player1.offset[1] - (player1.extent[1] / 2.0f) < -1.0f) {
        player1.offset[1] = -1.0f + (player1.extent[1] / 2.0f);
        player1Dp = 0;
    }

    // player2 && arena collision
    if (player2.offset[1] + (player2.extent[1] / 2.0f) > 1.0f) {
        player2.offset[1] = 1.0f - (player2.extent[1] / 2.0f);
        player2Dp = 0;
    }
    if (player2.offset[1] - (player2.extent[1] / 2.0f) < -1.0f) {
        player2.offset[1] = -1.0f + (player2.extent[1] / 2.0f);
        player2Dp = 0;
    }
}

static void drawRect(Rect_t rect) {
    MatIdentity(transform);
    MatTranslate(transform, V3(rect.offset[0], rect.offset[1], 0.0f));
    MatScale(transform, V3(rect.extent[0], rect.extent[1], 1.0f));

    glBindProgramPipeline(pipeline);
    glProgramUniformMatrix4fv(vsh, 0, 1, GL_FALSE, transform);

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glBindProgramPipeline(0);
}

int main(int argc, char** argv) {
    (void) argc;
    (void) argv;

    assert(glfwInit() == GLFW_TRUE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#else
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_FALSE);
#endif

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    GLFWwindow* win = glfwCreateWindow(1600, 900, "pong", 0, 0);
    assert(win);

    glfwMakeContextCurrent(win);

    assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == true);

    glfwSetWindowSizeCallback(win, resizeViewport);

    const Vertex_t vertices[] = {
        {{-0.5f, -0.5f, 0.0f}},
        {{ 0.5f, -0.5f, 0.0f}},
        {{-0.5f,  0.5f, 0.0f}},
        {{ 0.5f,  0.5f, 0.0f}},
    };

    const unsigned int indices[] = {
        0, 1, 2,
        1, 2, 3,
    };

    glCreateBuffers(1, &vbo);
    glNamedBufferStorage(vbo, sizeof(vertices), vertices, GL_DYNAMIC_STORAGE_BIT);

    glCreateBuffers(1, &ebo);
    glNamedBufferStorage(ebo, sizeof(indices), indices, GL_DYNAMIC_STORAGE_BIT);

    glCreateVertexArrays(1, &vao);
    glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex_t));
    glVertexArrayElementBuffer(vao, ebo);

    glEnableVertexArrayAttrib(vao, 0);

    glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex_t, position));

    glVertexArrayAttribBinding(vao, 0, 0);

    const char* vshSource = loadASCIIFile("shaders/vert.glsl", 0);
    const char* fshSource = loadASCIIFile("shaders/frag.glsl", 0);

    vsh = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &vshSource);
    fsh = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &fshSource);

    glCreateProgramPipelines(1, &pipeline);
    glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, vsh);
    glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fsh);

    free((void*)fshSource);
    free((void*)vshSource);
    
    {
        int windowWidth, windowHeight;
        glfwGetWindowSize(win, &windowWidth, &windowHeight);
        resizeViewport(win, windowWidth, windowHeight);
    }

    glfwShowWindow(win);

    float current = 0.0f, last = 0.0f, delta;
    while (!glfwWindowShouldClose(win)) {
        current = (float) glfwGetTime();
        delta = current - last;
        last = current;

        glClearBufferfv(GL_COLOR, 0, (float[]){0.1f, 0.1f, 0.1f, 1.0f});

        processInput(win);
        simulatePhysics(delta);

        drawRect(ball);
        drawRect(player1);
        drawRect(player2);

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glDeleteProgramPipelines(1, &pipeline);
    glDeleteProgram(fsh);
    glDeleteProgram(vsh);

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vbo);

    glfwDestroyWindow(win);
    glfwTerminate();

    return 0;
}
