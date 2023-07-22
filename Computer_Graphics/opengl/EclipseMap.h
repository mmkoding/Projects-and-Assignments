#ifndef ECLIPSEMAP_H
#define ECLIPSEMAP_H

#include <vector>
#include <GL/glew.h>
#include <iostream>
#include "glm/glm/ext.hpp"
#include "Shader.h"
#include <vector>
#include "glm/glm/glm.hpp"
#include <GLFW/glfw3.h>
#include <jpeglib.h>
#include <GL/glew.h>

#define PI 3.14159265359
using namespace std;


class EclipseMap {
private:


    float heightFactor = 0;
    float textureOffset = 0;
    float orbitDegree = 0;
    glm::vec3 lightPos = glm::vec3(0, 4000, 0);
    bool pKeyPressed = false;
    // DISPLAY SETTINGS
    enum displayFormatOptions {
        windowed = 1, fullScreen = 0
    };
    const char *windowName = "Ceng477 - HW3";
    int defaultScreenWidth = 1000;
    int defaultScreenHeight = 1000;
    int screenWidth = defaultScreenWidth;
    int screenHeight = defaultScreenHeight;
    int displayFormat = displayFormatOptions::windowed;
    // CAMERA SETTINGS
    float projectionAngle = 45; //fovy
    float aspectRatio = 1;
    float near = 0.1;
    float far = 10000;
    float startPitch = 180;
    float startYaw = 90;
    float startSpeed = 0;
    float pitch = startPitch;
    float yaw = startYaw;
    float speed = startSpeed;
    glm::vec3 cameraStartPosition = glm::vec3(0, 4000, 4000);
    glm::vec3 cameraStartDirection = glm::vec3(0, 0, -1);
    glm::vec3 cameraStartUp = glm::vec3(0, 0, 1);
    glm::vec3 cameraUp = cameraStartUp;
    glm::vec3 cameraPosition = cameraStartPosition;
    glm::vec3 cameraDirection = cameraStartDirection;
    glm::vec3 cameraLeft = glm::cross(cameraUp, cameraDirection);
    
public:
    unsigned int textureColor;
    unsigned int textureGrey;
    unsigned int VAO;
    unsigned int VBO, EBO;
    float imageHeight;
    float imageWidth;
    float radius = 600;
    int horizontalSplitCount = 250;
    int verticalSplitCount = 125;

    unsigned int moonTextureColor;
    unsigned int moonVAO;
    unsigned int moonVBO, moonEBO;
    float moonImageHeight;
    float moonImageWidth;
    float moonRadius = 162;

    std::vector<int> indices;
    std::vector<Vertex> vertices;
    
    std::vector<int> moonIndices;
    std::vector<Vertex> moonVertices;

    glm::mat4 MVP, M_model, M_view, M_projection;
    glm::mat4 initMVP, initM_model, initM_view, initM_projection;

    int uniformMVP; 
    int uniformHeightFactor; 
    int uniformCameraPosition; 
    int uniformHeightMap;
    int uniformTexture; 
    int uniformLightPosition; 
    int uniformTextureOffset;

    int coloredTextureWidth; 
    int coloredTextureHeight; 
    int textureGreyWidth; 
    int textureGreyHeight; 
    int mooncoloredTextureWidth; 
    int mooncoloredTextureHeight;


    //Flags for key functions

    

    GLFWwindow *openWindow(const char *windowName, int width, int height);

    void Render(char *coloredTexturePath, char *greyTexturePath, char *moonTexturePath);

    //void handleKeyPress(GLFWwindow *window, int key, int scancode, int action, int mods);

    void initColoredTexture(const char *filename, int *w, int *h);

    void initGreyTexture(const char *filename, int *w, int *h);

    void initMoonColoredTexture(const char *filename, int *w, int *h);
    
    //SETTERS GETTERS
    
    //EXTRA FUNCTIONS
    
    void initializeMoonBuffers();
    
    void initializeWorldBuffers();
    
    void configureGeometry();
    
    void setMoonUniforms(GLuint moonShaderID);

    void setWorldUniforms(GLuint worldShaderID);
    
    void drawMoon();

    void drawWorld();
    
    void moveLight(int toWhere);
    
    void updateScene();

    void changeDisplayFormat();

};

#endif
