#include "render.h"
#include "../clog.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <stdlib.h>

#include <assert.h>


#define MSAA_SAMPLES 16

struct window {
    GLFWwindow *win;
    uint width;
    uint height;

    int2_t position;

    uint2_t old_size;
};

const clog_level_t GLFW_ERROR = CLOG_REGISTER_LEVEL("GLFW Error", CLOG_COLOR_RED CLOG_COLOR_BOLD, CLOG_SEVERITY_FATAL);

void glfw_error_callback(int code, const char *msg) {
    clog(GLFW_ERROR, "Error %i: %s", code, msg);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
    window_t *win = (window_t*)glfwGetWindowUserPointer(window);

    win->width = width;
    win->height = height;
}  

int2_t get_window_size(window_t *win) {return int2(win->width, win->height);}

uint msaa_fbo = 0;

window_t *create_window(unsigned int width, unsigned int height, const char *title) {
    glfwSetErrorCallback(glfw_error_callback);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 8);

    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    window_t *ret = malloc(sizeof(struct window));
    clog_assert_m(ret != NULL, "Failed to allocate window_t.");

    ret->win = glfwCreateWindow(width, height, title, NULL, NULL);
    clog_assert_m(ret->win != NULL, "Failed to create window with GLFW.");

    ret->width = width;
    ret->height = height;

    glfwMakeContextCurrent(ret->win);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);

    glViewport(0, 0, width, height);
    glfwSetWindowUserPointer(ret->win, (void*)ret);
    glfwSetFramebufferSizeCallback(ret->win, framebuffer_size_callback);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

    return ret;
}

bool is_window_open(window_t *win) {return !glfwWindowShouldClose(win->win);}

float now = 0;
float prev = 0;
double delta_time = 0;


void update_window(window_t *win) {
    glfwSwapBuffers(win->win);
    glfwPollEvents();

    now = glfwGetTime();
    delta_time = now - prev;
    prev = now;
}

void close_window(window_t *win) {
    glfwDestroyWindow(win->win);
}

void clear_window(float3_t color) {
    glClearColor(color.x, color.y, color.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void toggle_fullscreen(window_t *win) {
    GLFWmonitor *monitor = glfwGetWindowMonitor(win->win);

    win->old_size = uint2(win->width, win->height);

    if (monitor == NULL) {
        glfwSetWindowMonitor(win->win, monitor, win->position.x, win->position.y, win->old_size.x, win->old_size.y, 0);
    }
    else {
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        glfwGetWindowPos(win->win, &win->position.x, &win->position.y);
        glfwSetWindowMonitor(win->win, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }
}

bool key_pressed(window_t *win, key key_code) {
    return glfwGetKey(win->win, (int)key_code) == GLFW_PRESS;
}
