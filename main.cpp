#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    out vec3 ourColor;
    void main()
    {
        gl_Position = vec4(aPos, 1.0);
        ourColor = aColor;
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    in vec3 ourColor;
    void main()
    {
        FragColor = vec4(ourColor, 1.0);
    }
)";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

int main()
{
    // Inicjalizacja GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Konfiguracja GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Tworzenie okna GLFW
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Kwadrat", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Inicjalizacja GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // Ustawienie widoku OpenGL
    glViewport(0, 0, 800, 600);

    // Kompilacja i linkowanie shader�w
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Definicja wierzcho�k�w kwadratu
    float vertices[] = {
        // Wierzcho�ki        // Kolory
        0.5f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f,  // Prawy g�rny
        0.5f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f,  // Prawy dolny
       -0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,  // Lewy dolny
       -0.5f,  0.5f, 0.0f,    1.0f, 1.0f, 0.0f   // Lewy g�rny
    };

    // Indeksy wierzcho�k�w dla kwadratu
    GLuint indices[] = {
        0, 1, 3,  // Pierwszy tr�jk�t
        1, 2, 3   // Drugi tr�jk�t
    };

    // Utworzenie i skonfigurowanie bufor�w OpenGL
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Wiazanie VAO
    glBindVertexArray(VAO);

    // Wiazanie VBO i przekazanie danych wierzcho�kowych
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Wiazanie EBO i przekazanie danych indeks�w
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Konfiguracja atrybut�w pozycji i kolor�w wierzcho�k�w
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Odwiazanie VAO
    glBindVertexArray(0);

    // Ustawienie callbacka na zmian� rozmiaru okna
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // P�tla g��wna
    while (!glfwWindowShouldClose(window))
    {
        // Sprawdzanie wej�cia
        processInput(window);

        // Czyszczenie bufora kolor�w
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // U�ycie shadera
        glUseProgram(shaderProgram);

        // Wiazanie VAO
        glBindVertexArray(VAO);

        // Rysowanie kwadratu
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Odwiazanie VAO
        glBindVertexArray(0);

        // Zamiana bufor�w
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Zwolnienie zasob�w
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // Zamkni�cie GLFW
    glfwTerminate();

    return 0;
}

// Funkcja obs�uguj�ca zmian� rozmiaru okna
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// Funkcja obs�uguj�ca wej�cie u�ytkownika
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}