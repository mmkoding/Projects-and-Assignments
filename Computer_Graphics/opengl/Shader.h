#ifndef SHADER_H
#define SHADER_H
using namespace std;

#include <iostream>
#include <string>
#include <fstream>
#include <jpeglib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm/vec3.hpp"
#include "glm/glm/vec2.hpp"

GLuint initShaders(const string& vertexShaderName, const string& fragmentShaderName);

GLuint initVertexShader(const string& filename);

GLuint initFragmentShader(const string& filename);

bool readDataFromFile(const string& fileName, string &data);

static GLFWwindow * window = nullptr;


struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texture;
};

#endif
