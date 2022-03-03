#ifndef BULLETS_H
#define BULLETS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "Shader.hpp"
#include "Bixo.hpp"

class Bullet
{

public:
    float vertexData[30] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,

        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f};

    float position[2] = {0.0f, 1.0f};
    float size[2] = {0.15f, 0.15f};
    bool existe = true;
    unsigned int VBO, VAO, texture;
    Shader shaderProgram = Shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    Bullet();
    void createBullet();
    void drawBullet();
    void setPosition(float x, float y);
    void move();
    bool stop();
    bool acertou(Bixo aliens[7][3]);
    void destroyBullet();
    void texturaBala(std::string textura);
};

#endif