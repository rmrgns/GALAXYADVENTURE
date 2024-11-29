#pragma once
#include "common.h"
#include "OBJRead.h"

char* filetobuf(const char* file);
void make_vertexShaders(GLuint& vertexShader, string shaderName);
void make_fragmentShaders(GLuint& fragmentShader, string shaderName);
GLvoid make_shaderProgram(GLuint& shaderProgramID, GLuint& vertexShader, GLuint& fragmentShader);
void CreateTriBuffer(GLuint& VAO, GLuint VBO[2], GLfloat triShape[9], GLfloat colors[9]);
void CreateRectBuffer(GLfloat vPositionList[], GLuint& VAO, GLuint& EBO, GLuint VBO[2], GLfloat colors[12]);

void CreateIndexBuffer(GLuint& VAO, GLuint VBO[2], GLuint& EBO, GLfloat point[12], GLfloat colors[12], int shape, unsigned int index[]);
void CreateModel(GLuint& VAO, GLuint VBO[2], GLuint& EBO, Model model, GLfloat color[], size_t sizeColor);