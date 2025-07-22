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
#include <Lsr3D/test/test5.h>
#include <iostream>
#include <iomanip>


// 帧时间统计变量
static int lastTime = 0;
static int frameCount = 0;
static float fps = 0.0f;
static float frameTime = 0.0f;

// 定义初始窗口大小
const int initialWidth = 1024;
const int initialHeight = 1024;

// 定义静态员变量
lsr3d::Renderer lsr3d::Renderer::instance(initialWidth, initialHeight);

void initializeOpenGL(int argc = 0, char** argv = nullptr);
void ResizeWindow(int width, int height);
void calculateFrameTime();
void displayFrameTime();
/**
 * @brief init draw such as model load
 *
 */
void Init(int width, int height);
void Display();

/**
 * @brief 计算帧时间和FPS
 */
void calculateFrameTime() {
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    int deltaTime = currentTime - lastTime;
    
    frameCount++;
    
    // 每500毫秒更新一次显示
    if (deltaTime >= 500) {
        fps = frameCount * 1000.0f / deltaTime;
        frameTime = deltaTime / (float)frameCount;
        
        std::cout << std::fixed << std::setprecision(2) 
                  << "FPS: " << fps 
                  << " | Frame Time: " << frameTime << " ms" << std::endl;
        
        lastTime = currentTime;
        frameCount = 0;
    }
}

/**
 * @brief 显示单帧渲染时间（每帧调用）
 */
void displayFrameTime() {
    static int previousTime = glutGet(GLUT_ELAPSED_TIME);
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    float singleFrameTime = currentTime - previousTime;
    
    // 可选：每隔一定帧数显示一次，避免输出过多
    static int displayCounter = 0;
    if (displayCounter % 60 == 0) { // 每60帧显示一次
        std::cout << "Single Frame Time: " << singleFrameTime << " ms" << std::endl;
    }
    displayCounter++;
    
    previousTime = currentTime;
}
/**
 * @brief 空闲回调函数，用于持续更新和重绘
 */
void idleCallback() {
    calculateFrameTime();
    displayFrameTime();
    glutPostRedisplay(); // 触发重绘
}

int main(int argc, char** argv) {
    // 初始化OpenGL
    initializeOpenGL(argc, argv);
    glutReshapeFunc(ResizeWindow);
    glutDisplayFunc(Display);
    glutIdleFunc(idleCallback); // 设置空闲回调
    
    // 初始化计时器
    lastTime = glutGet(GLUT_ELAPSED_TIME);
    
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
    glutInitWindowSize(initialWidth, initialHeight);
    glutCreateWindow("Soft Resterizator");
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, initialWidth, initialHeight, 0, -1, 1);
    Init(initialWidth, initialHeight);
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
    lsr3d::Renderer::instance.resize(width, height);
}
