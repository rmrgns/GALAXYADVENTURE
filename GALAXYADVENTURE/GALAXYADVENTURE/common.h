#pragma once
#ifndef CGCOMMON_H
#define CGCOMMON_H

#define _CRT_SECURE_NO_WARNINGS

#define PI 3.141592

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FPS 60.0f

// �ڵ�������
// �Լ�/����ü			-> lowerCammel
// ����					-> lower
// Ŭ����				-> UpperCammel
// #define				-> UPPER		


#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h> 
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include <vector>
#include <random>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <fstream>

using namespace std;

// ������ ũ��
struct windowSizeInfo
{
	int windowX = 800;
	int windowY = 800;
};

// ���� ����
struct BackgroundColor
{
	GLfloat r = 1.0f;
	GLfloat g = 1.0f;
	GLfloat b = 1.0f;
	GLfloat a = 1.0f;
};

static random_device rd;
static mt19937 gen(rd());

void RandomColor(float& colorR, float& colorG, float& colorB);
void RandomColor(glm::vec3 &color);
void RandomPosition(glm::vec3 &pos);

#endif // CGCOMMON_H