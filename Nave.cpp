#include "Nave.hpp"
#include "stb_image.h"

Nave::Nave() {}

void Nave::createNave()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // 1- bind the Vertex Array Object
    // 2- bind and set vertex buffer(s)
    // 3- configure vertex attributes(s)
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    // (location, size, GL_datatype, normalize, stride, pointer_to_start)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    //Texture coordinates attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Nave::drawNave()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    shaderProgram.use();
    glBindVertexArray(VAO);

    //float color[4] = {1.0f, 0.0f, 0.0f, 1.0f};

    //shaderProgram.setVec4("uColor", color);
    shaderProgram.setVec2("translate", position);
    int scaleLocation = glGetUniformLocation(shaderProgram.id, "scale");
    glUniform1f(scaleLocation, 0.15f);
    glDrawArrays(GL_TRIANGLES, 0, 6); // (primitiva, location, numero de vertices)
    tempo_atual = glfwGetTime();
}

void Nave::setPosition(float x, float y)
{
    position_ant[0] = position[0];
    position_ant[1] = position[1];
    position[0] = x;
    position[1] = y;
    tempo_antigo = tempo_atual;
    tempo_atual = glfwGetTime();
}

void Nave::atirar(int index)
{
    tiros[index].setPosition(position[0], position[1]);
}

float Nave::velocidade()
{
    float velocidade;

    velocidade = (position[0] - position_ant[0]) / (tempo_atual - tempo_antigo);

    return velocidade;
}

void Nave::textura(std::string textura)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(textura.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
}