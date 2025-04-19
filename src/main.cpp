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
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
void initiateSpin(GLFWwindow* window);
void processCamMovement(GLFWwindow* window);
void processCamReset(GLFWwindow* window);
void resetCamera();

float rotationSpeed = 1.0f;
float lastX = 800.0f / 2.0f;
float lastY = 600.0f / 2.0f;
float fov = 45.0f;
float lastFrame = 0.0f;
float yaw = -90.0f;
float pitch = 0.0f;
const float speedIncrement = 0.01f;
bool isRotating = false;
bool firstMouse = true;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::vec3 initialPos = cameraPos;
glm::vec3 initialFront = cameraFront;
glm::vec3 initialUp = cameraUp;


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
    glfwMaximizeWindow(window);
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);

    Shader newShader("../../../src/vshader.vert", "../../../src/fshader.frag");

    const std::array vertices = {
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f
    };
    
    constexpr std::array staticVertices = {
        1.0f, 0.9f, 0.0f, 1.0f, 1.0f,
        1.0f, -0.9f, 0.0f, 1.0f, 0.0f, 
        -1.0f, -0.9f, 0.0f, 0.0f, 0.0f,
        -1.0f, 0.9f, 0.0f, 0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    
    uint32_t VBO, VAO, staticVBO, staticVAO, EBO;
    
    glGenBuffers(1, &staticVBO);
    glGenBuffers(1, &EBO);
    
    glGenVertexArrays(1, &staticVAO);
    
    glBindVertexArray(staticVAO);
    

    //staticmodel
    glBindBuffer(GL_ARRAY_BUFFER, staticVBO);   
    glBufferData(GL_ARRAY_BUFFER, sizeof(staticVertices), staticVertices.data(), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glVertexAttrib3f(1, 1.0f, 1.0f, 1.0f);
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //movingmodel
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW);
    

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    

    uint32_t texture1, texture2, texture3, texture4;
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

    glGenTextures(1, &texture4);
    glBindTexture(GL_TEXTURE_2D, texture4);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("../../../assets/gargantua.png", &width, &height, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to Load fourth texture" << std::endl;
    }

    stbi_image_free(data);

    newShader.use();
    glUniform1i(glGetUniformLocation(newShader.ID, "ourTexture1"), 0);
    glUniform1i(glGetUniformLocation(newShader.ID, "ourTexture2"), 1);
    glUniform1i(glGetUniformLocation(newShader.ID, "ourTexture3"), 2);
    glUniform1i(glGetUniformLocation(newShader.ID, "ourTexture4"), 3);
    glm::mat4 projMat = glm::mat4(1.0f);
    projMat = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    

 /*   newShader.setInt("texture1", 0);
    newShader.setInt("texture2", 1);*/


    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        processCamMovement(window);
        processInput(window);
        initiateSpin(window);
        processCamReset(window);
        float rotAngle = 0.0f;

        if (isRotating) {
            rotAngle = glfwGetTime() * rotationSpeed;
            rotationSpeed += speedIncrement;
        }      
        

        glClearColor(0.2f, 0.2f, 0.1f, 0.5f);
        /*glClear(GL_COLOR_BUFFER_BIT);*/
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture3);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, texture4);

        
        newShader.use();

        glm::mat4 camViewMat = glm::mat4(1.0f);
        camViewMat = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glUniformMatrix4fv(glGetUniformLocation(newShader.ID, "viewM"), 1, GL_FALSE, glm::value_ptr(camViewMat));
       

          
        glUniform1i(glGetUniformLocation(newShader.ID, "useVertexColor"), 0);
        glUniform3f(glGetUniformLocation(newShader.ID, "tempColor"), 0.5f, 0.5f, 0.5f);
        glUniform1i(glGetUniformLocation(newShader.ID, "useStaticTexture"), 1);

        //staticModel
        glm::mat4 staticMat = glm::mat4(1.0f);
        staticMat = glm::translate(staticMat, glm::vec3(0.0f, 0.0f, -9.0f));
        glm::mat4 staticViewMat = glm::mat4(1.0f);
        staticViewMat = glm::translate(staticViewMat, glm::vec3(0.0f, 0.0f, -9.0f));

        //common projection mat
        

        glUniformMatrix4fv(glGetUniformLocation(newShader.ID, "transform"), 1, GL_FALSE, glm::value_ptr(staticMat));
        //glUniformMatrix4fv(glGetUniformLocation(newShader.ID, "viewM"), 1, GL_FALSE, glm::value_ptr(staticViewMat));
        glUniformMatrix4fv(glGetUniformLocation(newShader.ID, "projM"), 1, GL_FALSE, glm::value_ptr(projMat));

        glBindVertexArray(staticVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //////static model config ends

        float timeVal = glfwGetTime();
        float redVal = sin(timeVal) / 2.0f + 0.5f;
        float greenVal = cos(timeVal) / 2.0f + 0.5f;
        float blueVal = tan(timeVal) / 2.0f + 0.5f;

        float weightVal1 = sin(timeVal) / 2.0f + 0.5f;
        float weightVal2 = cos(timeVal) / 2.0f + 0.2f;
        float weightVal3 = sin(timeVal) / 2.0f;
        glUniform1i(glGetUniformLocation(newShader.ID, "useVertexColor"), 1);
        glUniform1i(glGetUniformLocation(newShader.ID, "useStaticTexture"), 0);
        glUniform4f(glGetUniformLocation(newShader.ID, "finalColor"), redVal, greenVal, blueVal, 1.0f);
        glUniform1f(glGetUniformLocation(newShader.ID, "weight1"), weightVal1);
        glUniform1f(glGetUniformLocation(newShader.ID, "weight2"), weightVal2);
        glUniform1f(glGetUniformLocation(newShader.ID, "weight3"), weightVal3);


        //moving Model
        //model 
        glm::mat4 transMat = glm::mat4(1.0f);
        //transMat = glm::translate(transMat, glm::vec3(-0.5f, 0.0f, 0.0f));
        transMat = glm::translate(transMat, glm::vec3(-cos(timeVal) * 4.0f, 2.0f * (0.5f - cos(timeVal)), -sin(timeVal) * 4.0f - 12.0f));

        transMat = glm::rotate(transMat, rotAngle * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        //view
        glm::mat4 viewMat = glm::mat4(1.0f);
        viewMat = glm::translate(viewMat, glm::vec3(-cos(timeVal) * 4.0f, 2.0f * (0.5f - cos(timeVal)), -sin(timeVal) * 4.0f - 12.0f));
        
        
        glUniformMatrix4fv(glGetUniformLocation(newShader.ID, "transform"), 1, GL_FALSE, glm::value_ptr(transMat));
        //glUniformMatrix4fv(glGetUniformLocation(newShader.ID, "viewM"), 1, GL_FALSE, glm::value_ptr(viewMat));
        glUniformMatrix4fv(glGetUniformLocation(newShader.ID, "projM"), 1, GL_FALSE, glm::value_ptr(projMat));
        
        glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        //////moving model config ends

        glm::mat4 transMat2 = glm::mat4(1.0f);
        transMat2 = glm::translate(transMat2, glm::vec3(sin(timeVal) * 4.0f, 5.0f * (0.5f - cos(timeVal)), sin(timeVal) * 5.0f - 9.0f));
        transMat2 = glm::rotate(transMat2, rotAngle * glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 viewMat2 = glm::mat4(1.0f);
        viewMat2 = glm::translate(viewMat2, glm::vec3(sin(timeVal) * 4.0f, 5.0f * (0.5f - cos(timeVal)), sin(timeVal) * 5.0f - 9.0f));
        glUniformMatrix4fv(glGetUniformLocation(newShader.ID, "transform"), 1, GL_FALSE, glm::value_ptr(transMat2));
        //glUniformMatrix4fv(glGetUniformLocation(newShader.ID, "viewM"), 1, GL_FALSE, glm::value_ptr(viewMat2));
        glUniformMatrix4fv(glGetUniformLocation(newShader.ID, "projM"), 1, GL_FALSE, glm::value_ptr(projMat));

        glDrawArrays(GL_TRIANGLES, 0, 36);

        glm::mat4 transMat3 = glm::mat4(1.0f);
        transMat3 = glm::translate(transMat3, glm::vec3(sin(timeVal) * 6.0f, 0.0f, -cos(timeVal) * 4.0f - 11.0f));
        transMat3 = glm::rotate(transMat3, rotAngle * glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

       /* glm::mat4 viewMat3 = glm::mat4(1.0f);
        viewMat3 = glm::translate(viewMat3, glm::vec3(sin(timeVal) * 3.0f, 0.0f, -cos(timeVal) * 2.0f - 11.0f));*/
        glUniformMatrix4fv(glGetUniformLocation(newShader.ID, "transform"), 1, GL_FALSE, glm::value_ptr(transMat3));
        //glUniformMatrix4fv(glGetUniformLocation(newShader.ID, "viewM"), 1, GL_FALSE, glm::value_ptr(viewMat3));
        glUniformMatrix4fv(glGetUniformLocation(newShader.ID, "projM"), 1, GL_FALSE, glm::value_ptr(projMat));

        
        

        glDrawArrays(GL_TRIANGLES, 0, 36);
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
    
}


void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

}

void initiateSpin(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
        isRotating = true;
}

void processCamMovement(GLFWwindow* window) {
    const float cameraSpeed = 0.05f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void resetCamera() {
    cameraPos = initialPos;
    cameraFront = initialFront;
    cameraUp = initialUp;

    yaw = -90.0f;
    pitch = 0.0f;

    lastX = 800.0f / 2.0f;
    lastY = 600.0f / 2.0f;
    firstMouse = true;
}

void processCamReset(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        resetCamera();
}