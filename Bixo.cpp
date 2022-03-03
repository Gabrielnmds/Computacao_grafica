#include "Bixo.hpp"
#include "stb_image.h"

Bixo::Bixo() {}

void Bixo::createBixo()
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

void Bixo::drawBixo()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    if (vivo)
    {
        shaderProgram.use();
        glBindVertexArray(VAO);

        //float color[4] = {0.5f, 0.0f, 1.0f, 1.0f};

        //shaderProgram.setVec4("uColor", color);
        shaderProgram.setVec2("translate", position);

        int scaleLocation = glGetUniformLocation(shaderProgram.id, "scale");
        glUniform1f(scaleLocation, 0.19f);
        glDrawArrays(GL_TRIANGLES, 0, 6); // (primitiva, location, numero de vertices)
    }
}

void Bixo::setPosition(float x, float y)
{
    position[0] = x;
    position[1] = y;
}

void Bixo::setInicialPosition(float x, float y)
{
    position[0] = x;
    position[1] = y;
    init_x = delta_y = x;
    init_y = y;
}

void Bixo::move()
{
    if (vivo)
    {
        setPosition(init_x + delta_x, init_y + sin(delta_y * 3) / 8);
        delta_x += x_step;
        delta_y += 0.005f;
        if (init_x + delta_x > init_x + 0.15f)
        {
            x_step = -x_step;
        }
        if (delta_x + init_x < init_x - 0.15f)
        {
            x_step = -x_step;
        }
        init_y -= 0.0005f;
    }
}

void Bixo::destroyBixo()
{
    vivo = false;
    setPosition(1.0f, 1.0f);
}

void Bixo::texturaBixo(std::string textura)
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

bool Bixo::atingeNave(float nave_pos[2], float nave_size[2])
{
    float bixo_x[2] = {position[0] + (size[0]) / 2, position[0] - (size[0]) / 2};
    float bixo_y[2] = {position[1] + (size[1]) / 2, position[1] - (size[1]) / 2};
    float nave_x[2] = {nave_pos[0] + (nave_size[0]) / 2,
                       nave_pos[0] - (nave_size[0]) / 2};
    float nave_y[2] = {nave_pos[1] + (nave_size[1]) / 2,
                       nave_pos[1] - (nave_size[1]) / 2};
    if (bixo_x[0] >= nave_x[1] && bixo_x[1] <= nave_x[0])
    {
        if (bixo_y[0] >= nave_y[1] && bixo_y[1] <= nave_y[0])
            return true;
    }
    return false;
}