#ifndef TEXTO_H
#define TEXTO_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "Shader.hpp"

class Texto
{

public:
    float vertexData[30] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,

        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f};

    float position[2] = {0.0f, 0.0f};
    bool aparece = true;
    float criada = 0.0;
    unsigned int VBO, VAO, texture;
    Shader shaderProgram = Shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    Texto();
    void createTexto();
    void drawTexto();
    void texturaTexto(std::string textura);
};

#endif