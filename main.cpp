/**
 * @file main.cpp
 * @author dion (you@domain.com) zhywyt (zhywyt@yeah.net)
 * @brief Resterizator
 * @details This is a soft rasterizer that implements the basic rendering pipeline.
 * @version 0.1
 * @date 2025-07-05
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <GL/freeglut.h>
#include <test/test2.h>


void initializeOpenGL(int argc = 0, char** argv = nullptr);
void ResizeWindow(int width, int height);
void Display();
int main(int argc, char** argv) {
    // 初始化OpenGL
    initializeOpenGL(argc, argv);
    glutReshapeFunc(ResizeWindow);
    glutDisplayFunc(Display);
    glutMainLoop();
    return 0;
}

/**
 * @brief Initialize OpenGL
 *
 * @param argc Number of program arguments
 * @param argv Program argument array
 */
void initializeOpenGL(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(512, 512);
    glutCreateWindow("Soft Resterizator");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 512, 512, 0, -1, 1);
}
/**
 * @brief Call back for window resize
 *
 * @param width
 * @param height
 */
void ResizeWindow(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
}
