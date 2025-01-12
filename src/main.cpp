#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include <iostream>
#include <array>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void initiateSpin(GLFWwindow* window);

float rotationSpeed = 1.0f;
const float speedIncrement = 0.001f;
bool isRotating = false;


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create glfw window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to load GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    Shader newShader("../../../src/vshader.vert", "../../../src/fshader.frag");

    constexpr std::array<float, 32> vertices = {
        0.4f, 0.8f, 0.0f, 1.0f, 0.0f, 0.0f, 2.0f, 2.0f,
        0.4f, -0.8f, 0.0f, 0.0f, 1.0f, 0.0f, 2.0f, 0.0f, 
        -0.4f, -0.8f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.4f, 0.8f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 2.0f
    };

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    
    uint32_t VBO, VAO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    

    uint32_t texture1, texture2, texture3;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("../../../assets/someImage.jpeg", &width, &height, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to Load texture" << std::endl;
    }

    stbi_image_free(data);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("../../../assets/pepe.png", &width, &height, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load second texture" << std::endl;
    }

    stbi_image_free(data);

    glGenTextures(1, &texture3);
    glBindTexture(GL_TEXTURE_2D, texture3);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("../../../assets/text.png", &width, &height, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load third texture" << std::endl;
    }

    stbi_image_free(data);

    newShader.use();
    glUniform1i(glGetUniformLocation(newShader.ID, "ourTexture1"), 0);
    glUniform1i(glGetUniformLocation(newShader.ID, "ourTexture2"), 1);
    glUniform1i(glGetUniformLocation(newShader.ID, "ourTexture3"), 2);

    

 /*   newShader.setInt("texture1", 0);
    newShader.setInt("texture2", 1);*/


    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        initiateSpin(window);
        
        float rotAngle = 0.0f;

        if (isRotating) {
            rotAngle = glfwGetTime() * rotationSpeed;
            rotationSpeed += speedIncrement;
        }
        
        

        /*glClearColor(0.2f, 0.2f, 0.1f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT);*/
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture3);

        
        newShader.use();
        float timeVal = glfwGetTime();
        float redVal = sin(timeVal) / 2.0f + 0.5f;
        float greenVal = cos(timeVal) / 2.0f + 0.5f;
        float blueVal = tan(timeVal) / 2.0f + 0.5f;

        float weightVal1 = sin(timeVal) / 2.0f + 0.5f;
        float weightVal2 = cos(timeVal) / 2.0f + 0.2f;
        float weightVal3 = sin(timeVal) / 2.0f;
        glUniform4f(glGetUniformLocation(newShader.ID, "finalColor"), redVal, greenVal, blueVal, 1.0f);
        glUniform1f(glGetUniformLocation(newShader.ID, "weight1"), weightVal1);
        glUniform1f(glGetUniformLocation(newShader.ID, "weight2"), weightVal2);
        glUniform1f(glGetUniformLocation(newShader.ID, "weight3"), weightVal3);

        glm::mat4 transMat = glm::mat4(1.0f);
        //transMat = glm::translate(transMat, glm::vec3(-0.5f, 0.0f, 0.0f));
        transMat = glm::rotate(transMat, rotAngle, glm::vec3(0.0f, 0.0f, 1.0f));
        
        
        glUniformMatrix4fv(glGetUniformLocation(newShader.ID, "transform"), 1, GL_FALSE, glm::value_ptr(transMat));
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

}

void initiateSpin(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
        isRotating = true;
}