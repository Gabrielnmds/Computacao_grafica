#ifndef BIXO_H
#define BIXO_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "Shader.hpp"
//#include "Nave.hpp"
//#include "Bullet.hpp"

class Bixo
{

public:
    float vertexData[30] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,

        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f};

    bool vivo = true;
    float position[2];
    float size[2] = {0.1f, 0.1f};
    float init_x, init_y;
    float delta_x = 0.0, delta_y = 0.0, x_step = 0.005f;
    unsigned int VBO, VAO, texture;
    Shader shaderProgram = Shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    Bixo();
    void createBixo();
    void drawBixo();
    void setPosition(float x, float y);
    void setInicialPosition(float x, float y);
    void move();
    void destroyBixo();
    void texturaBixo(std::string textura);
    bool atingeNave(float nave_pos[2], float nave_size[2]);
};

#endif