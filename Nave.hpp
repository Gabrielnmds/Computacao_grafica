#ifndef NAVE_H
#define NAVE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "Shader.hpp"
#include "Bullet.hpp"

class Nave
{

public:
    float vertexData[30] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,

        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f};

    float position[2], position_ant[2], tempo_atual = 0.0, tempo_antigo = 0.0;
    Bullet tiros[21];
    float size[2] = {0.15f, 0.15f};
    unsigned int VBO, VAO, texture;
    Shader shaderProgram = Shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    Nave();
    void createNave();
    void drawNave();
    void atirar(int index);
    void setPosition(float x, float y);
    float velocidade();
    void textura(std::string textura);
};

#endif