#pragma once
#ifndef CGCOMMON_H
#define CGCOMMON_H

#define _CRT_SECURE_NO_WARNINGS

#define PI 3.141592

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FPS 60

// 코딩컨벤션
// 함수/구조체			-> lowerCammel
// 변수					-> lower
// 클래스				-> UpperCammel
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

// 윈도우 크기
struct windowSizeInfo
{
	int windowX = 800;
	int windowY = 800;
};

// 배경색 선택
struct BackgroundColor
{
	GLfloat r = 1.0f;
	GLfloat g = 1.0f;
	GLfloat b = 1.0f;
	GLfloat a = 1.0f;
};

static random_device rd;
static mt19937 gen(rd());

bool CheckColide_BS(glm::vec3 sphere1, glm::vec3 sphere2, float radius1, float radius2);

void RandomColor(float& colorR, float& colorG, float& colorB);

#endif // CGCOMMON_H