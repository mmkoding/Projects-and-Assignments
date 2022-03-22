#version 430

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexNormal;
layout(location = 2) in vec2 VertexTex;
 
uniform vec3 cameraPos;
uniform vec3 lightPos;
 
uniform mat4 MVP;
uniform float textureOffset;

out vec2 textureCoordinate;
out vec3 vertexNormal;

out vec3 LightVector;
out vec3 CameraVector;


void main() {
    textureCoordinate = VertexTex;
    textureCoordinate.x += textureOffset * (1.0 / 250.0);
    vertexNormal = VertexNormal;

    vec3 resultantPos = VertexPosition;
    CameraVector = normalize(cameraPos - resultantPos);
    LightVector = normalize(lightPos - resultantPos);
    gl_Position = MVP * vec4(resultantPos.xyz, 1.0f);
}