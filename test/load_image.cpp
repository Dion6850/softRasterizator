/**
 * @file load_image.cpp
 * @author dion (hduer_zdy@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2025/7/18
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef LOAD_IMAGE_H
#define LOAD_IMAGE_H

#include <stb/stb_image.h>
#include <GL/glut.h>
#include <iostream>
GLuint textureID;

inline void loadTexture(const char* filename)
{
    // 翻转图像以匹配OpenGL的坐标系统
    stbi_set_flip_vertically_on_load(true);
    
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);
    if (!data)
    {
        std::cerr << "Failed to load image: " << filename << std::endl;
        return;
    }

    std::cout << "Image loaded: " << width << "x" << height << ", Channels: " << channels << std::endl;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    GLenum format = GL_RGB;
    if (channels == 4) format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
}

inline void Display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glBegin(GL_QUADS);
    glTexCoord2f(0.f, 0.f);
    glVertex2f(-1.f, -1.f);
    glTexCoord2f(1.f, 0.f);
    glVertex2f(1.f, -1.f);
    glTexCoord2f(1.f, 1.f);
    glVertex2f(1.f, 1.f);
    glTexCoord2f(0.f, 1.f);
    glVertex2f(-1.f, 1.f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(512, 512);
    glutCreateWindow("stb_image texture");

    loadTexture("../assets/test.jpg"); // 替换成你实际的图片文件

    glutDisplayFunc(Display);
    glutMainLoop();

    return 0;
}

#endif //LOAD_IMAGE_H
