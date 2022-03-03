#include "Bullet.hpp"
#include "stb_image.h"

Bullet::Bullet() {}

void Bullet::createBullet()
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

void Bullet::drawBullet()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    shaderProgram.use();
    glBindVertexArray(VAO);

    //float color[4] = {1.0f, 1.0f, 1.0f, 1.0f};

    //shaderProgram.setVec4("uColor", color);
    shaderProgram.setVec2("translate", position);

    //int translateLocation = glGetUniformLocation(shaderProgram.id, "translate");
    //glUniform2f(translateLocation, 0.5f, 0.4f);
    int scaleLocation = glGetUniformLocation(shaderProgram.id, "scale");
    glUniform1f(scaleLocation, 0.15f);
    glDrawArrays(GL_TRIANGLES, 0, 6); // (primitiva, location, numero de vertices)
}

void Bullet::setPosition(float x, float y)
{
    position[0] = x;
    position[1] = y;
}

void Bullet::move()
{   
    if(existe)
        setPosition(position[0], position[1] + 0.07f);
    else
        setPosition(position[0], 1.0f);
}

bool Bullet::stop()
{
    if (position[1] >= 1.0f)
    {
        return true;
    }
    return false;
}

bool Bullet::acertou(Bixo aliens[7][3])
{
    float tiro_x[2] = {position[0] + (size[0]) / 2, position[0] - (size[0]) / 2};
    float tiro_y[2] = {position[1] + (size[1]) / 2, position[1] - (size[1]) / 2};
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            float alien_x[2] = {aliens[i][j].position[0] + (aliens[i][j].size[0]) / 2,
                                aliens[i][j].position[0] - (aliens[i][j].size[0]) / 2};
            float alien_y[2] = {aliens[i][j].position[1] + (aliens[i][j].size[1]) / 2,
                                aliens[i][j].position[1] - (aliens[i][j].size[1]) / 2};
            if (tiro_x[0] >= alien_x[1] && tiro_x[1] <= alien_x[0])
            {
                if (tiro_y[0] >= alien_y[1] && tiro_y[1] <= alien_y[0])
                {
                    aliens[i][j].destroyBixo();
                    destroyBullet();
                    return true;
                }
            }
        }
    }
    return false;
}

void Bullet::destroyBullet()
{
    existe = false;
}

void Bullet::texturaBala(std::string textura)
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