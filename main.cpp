#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Windows.h>
#include <string>
#include <iostream>
#include <cmath>
#include "Shader.hpp"
#include "Nave.hpp"
#include "Bixo.hpp"
#include "Texto.hpp"
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, Nave *nave, Bixo inimigos[7][3]);
static void cursorPositionCallback(GLFWwindow *window, double xPos, double yPos);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void imprimePosicoes(Nave *nave, Bixo inimigos[7][3]);

// Tamanho da janela
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
bool ATIROU = false;
bool PAUSE = false;
bool RESTART = false;
bool FRAME = false;

int main()
{
    // inicialização e configuração de contexto
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // criação da janela e verificação de erros
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Invasores Espaciais", NULL, NULL);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    if (window == NULL)
    {
        std::cout << "Falha na criacao da janela" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // GLAD: Carregamento dos ponteiros de funções para utilizar OpenGL moderno
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Falha na inicializacao do GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window))
    {

        Shader shaderProgram("shaders/vertex.glsl", "shaders/fragment.glsl");

        //cria a nave e seu arsenal
        Nave nave;
        nave.createNave();
        nave.textura("nave.png");

        //cria textos do jogo
        Texto start, you_won, game_over;
        start.createTexto();
        start.texturaTexto("start.png");
        you_won.createTexto();
        you_won.texturaTexto("you_won.png");
        you_won.aparece = false;
        game_over.createTexto();
        game_over.texturaTexto("game_over.png");
        game_over.aparece = false;

        for (int i = 0; i < 21; i++)
        {
            nave.tiros[i].createBullet();
            nave.tiros[i].texturaBala("missil.png");
        }

        float x = -0.6f, y = 0.6f;
        Bixo inimigos[7][3];
        for (int i = 0; i < 7; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                inimigos[i][j].createBixo();
                if (j == 1)
                    inimigos[i][j].setInicialPosition(x + 0.07f, y);
                else
                    inimigos[i][j].setInicialPosition(x, y);
                if (i == 0 || i == 3)
                    inimigos[i][j].texturaBixo("bixo4.png");
                else if (i == 1 || i == 4)
                    inimigos[i][j].texturaBixo("bixo2.png");
                else if (i == 2 || i == 5)
                    inimigos[i][j].texturaBixo("bixo3.png");
                else
                    inimigos[i][j].texturaBixo("bixo1.png");
                y -= 0.2f;
            }
            x += 0.2f;
            y = 0.6f;
        }

        int index = 0;

        // Loop da aplicação
        while (!glfwWindowShouldClose(window))
        {
            // Input handling
            processInput(window, &nave, inimigos);

            // Define a cor de fundo da janela
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            // Limpa algum buffer específico
            glClear(GL_COLOR_BUFFER_BIT); // A limpeza do framebuffer de cor será feita com a cor definida em glClearColor

            start.drawTexto();
            if (glfwGetTime() - start.criada >= 1.0)
                start.aparece = false;

            nave.drawNave();
            for (int i = 0; i < 21; i++)
            {
                if (nave.tiros[i].existe)
                {
                    nave.tiros[i].drawBullet();
                    if (PAUSE == false)
                        nave.tiros[i].move();
                    nave.tiros[i].acertou(inimigos);
                }
                else
                    continue;
            }

            for (int i = 0; i < 7; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    inimigos[i][j].drawBixo();
                    if (PAUSE == false)
                    {
                        inimigos[i][j].move();
                        if (inimigos[i][j].position[1] <= -0.7f || inimigos[i][j].atingeNave(nave.position, nave.size))
                        {
                            game_over.aparece = true;
                            PAUSE = true;
                        }
                    }
                }
            }
            int contador = 0;
            for (int i = 0; i < 7; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    if (!inimigos[i][j].vivo)
                        contador++;
                }
            }
            if (contador == 21)
            {
                you_won.aparece = true;
                PAUSE = true;
            }
            else
                contador = 0;

            game_over.drawTexto();
            you_won.drawTexto();

            // Faz a troca do framebuffer antigo para o novo (double buffer)
            glfwSwapBuffers(window);

            // Captura eventos de IO (movimento de mouse, teclas pressionadas, etc)
            glfwPollEvents();

            if (RESTART == true)
            {
                RESTART = false;
                PAUSE = false;
                break;
            }

            if (ATIROU == true)
            {
                nave.atirar(index);
                ATIROU = false;
                index++;
                index = index % 21;
            }

            if (FRAME == true)
            {

                PAUSE = false;
                // Input handling
                processInput(window, &nave, inimigos);

                // Define a cor de fundo da janela
                glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
                // Limpa algum buffer específico
                glClear(GL_COLOR_BUFFER_BIT); // A limpeza do framebuffer de cor será feita com a cor definida em glClearColor

                nave.drawNave();
                for (int i = 0; i < 21; i++)
                {
                    if (nave.tiros[i].existe)
                    {
                        nave.tiros[i].drawBullet();
                        nave.tiros[i].move();
                        nave.tiros[i].acertou(inimigos);
                    }
                    else
                        continue;
                }

                for (int i = 0; i < 7; i++)
                {
                    for (int j = 0; j < 3; j++)
                    {
                        inimigos[i][j].drawBixo();
                        inimigos[i][j].move();
                    }
                }

                // Faz a troca do framebuffer antigo para o novo (double buffer)
                glfwSwapBuffers(window);

                // Captura eventos de IO (movimento de mouse, teclas pressionadas, etc)
                glfwPollEvents();

                if (RESTART == true)
                {
                    RESTART = false;
                    break;
                }

                if (ATIROU == true)
                {
                    nave.atirar(index);
                    ATIROU = false;
                    index++;
                    index = index % 21;
                }
                FRAME = false;
                PAUSE = true;
            }
        }

        // Desalocar memória dos objetos instanciados
        // Faz a limpeza dos recursos utilizados pelo GLFW
    }
    glfwTerminate();
    return 0;
}
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window, Nave *nave, Bixo inimigos[7][3])
{
    double xPos, yPos;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    glfwGetCursorPos(window, &xPos, &yPos);

    if (PAUSE == false)
    {
        if (2 * (float)(xPos) / WIDTH - 1 > 0.8f)
        {
            nave->setPosition(0.8f, -0.7f);
        }
        else if (2 * (float)(xPos) / WIDTH - 1 < -0.8f)
        {
            nave->setPosition(-0.8f, -0.7f);
        }
        else
            nave->setPosition(2 * (float)(xPos) / WIDTH - 1, -0.7f);
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        if (PAUSE == false)
            ATIROU = true;
        Sleep(18);
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        PAUSE = !PAUSE;
        Sleep(100);
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        RESTART = true;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS)
    {
        PAUSE = true;
        FRAME = true;
        imprimePosicoes(nave, inimigos);
        Sleep(150);
    }
}

// Função executada toda vez que há um redimensionamento de janela
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void cursorPositionCallback(GLFWwindow *window, double xPos, double yPos) {}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {}

void imprimePosicoes(Nave *nave, Bixo inimigos[7][3])
{
    std::cout << "COORDENADAS DA NAVE: x = " << nave->position[0] << ", y = " << nave->position[1] << std::endl;
    std::cout << "VELOCIDADE DA NAVE:" << nave->velocidade() << std::endl;
    int alien = 0;
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            alien++;
            std::cout << "COORDENADAS DO ALIEN " << alien << ": x = " << inimigos[i][j].position[0] << ", y = " << inimigos[i][j].position[1] << std::endl;
            std::cout << "VELOCIDADE DO ALIEN " << alien << ": 0.0005 " << std::endl;
        }
    }
}