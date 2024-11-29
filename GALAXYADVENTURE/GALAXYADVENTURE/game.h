#pragma once
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include "player.h"

Player player;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.5f, 5.0f); //카메라 위치
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //카메라 바라보는 방향
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); //카메라 위쪽 방향

void Game_Start()
{
	player = Player();
}

void Game_Timer()
{

}

void Game_Keyboard(unsigned char key, int x, int y)
{

}

void Game_Mouse(int button, int state, float x, float y)
{

}