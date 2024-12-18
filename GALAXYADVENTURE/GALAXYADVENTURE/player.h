#pragma once

#include "Shape.h"

const float shipsize = 0.5f;
const float shipspeed = 1.f;

enum Keyboard_type { KEY_DOWN, KEY_UP };

class Player : public Shape
{
private:
	Model2 model;
	const char* filename = "OBJ/Plane.obj";
public:
	glm::vec3 speed;
	glm::vec3 angle;

	bool crash;
	bool erase;

	Player()
	{
		//int index = 0;
		////기체 아랫면
		//shapecoord[index++] = glm::vec3(-shipsize, -shipsize, shipsize);
		//shapecoord[index++] = glm::vec3(shipsize, -shipsize, shipsize);
		//shapecoord[index++] = glm::vec3(0, -shipsize, -shipsize);

		////기체 윗면
		//shapecoord[index++] = glm::vec3(-shipsize, shipsize, shipsize);
		//shapecoord[index++] = glm::vec3(shipsize, shipsize, shipsize);
		//shapecoord[index++] = glm::vec3(0, shipsize, -shipsize);

		////기체 뒷면
		//shapecoord[index++] = glm::vec3(-shipsize, -shipsize, shipsize);
		//shapecoord[index++] = glm::vec3(shipsize, -shipsize, shipsize);
		//shapecoord[index++] = glm::vec3(shipsize, shipsize, shipsize);

		//shapecoord[index++] = glm::vec3(shipsize, shipsize, shipsize);
		//shapecoord[index++] = glm::vec3(-shipsize, shipsize, shipsize);
		//shapecoord[index++] = glm::vec3(-shipsize, -shipsize, shipsize);

		////기체 왼쪽
		//shapecoord[index++] = glm::vec3(-shipsize, -shipsize, shipsize);
		//shapecoord[index++] = glm::vec3(-shipsize, shipsize, shipsize);
		//shapecoord[index++] = glm::vec3(0, shipsize, -shipsize);

		//shapecoord[index++] = glm::vec3(0, shipsize, -shipsize);
		//shapecoord[index++] = glm::vec3(0, -shipsize, -shipsize);
		//shapecoord[index++] = glm::vec3(-shipsize, -shipsize, shipsize);

		////기체 오른쪽
		//shapecoord[index++] = glm::vec3(shipsize, -shipsize, shipsize);
		//shapecoord[index++] = glm::vec3(shipsize, shipsize, shipsize);
		//shapecoord[index++] = glm::vec3(0, shipsize, -shipsize);

		//shapecoord[index++] = glm::vec3(0, shipsize, -shipsize);
		//shapecoord[index++] = glm::vec3(0, -shipsize, -shipsize);
		//shapecoord[index++] = glm::vec3(shipsize, -shipsize, shipsize);

		//for (int i = 0; i < 6; ++i)
		//	shapecolor[i] = glm::vec3(0.0f, 1.0f, 0.0f);

		//for (int i = 6; i < 12; ++i)
		//	shapecolor[i] = glm::vec3(0.0f, 1.0f, 1.0f);

		//for (int i = 12; i < index; ++i)
		//	shapecolor[i] = glm::vec3(1.0f, 0.0f, 1.0f);

		model.loadFromFile(filename);
		shapecolor[0] = glm::vec3(1.0f);

		translation = glm::vec3(0.0);
		rotation = glm::vec3(0.0);
		revolution = glm::vec3(0.0);
		scaling = glm::vec3(1.0);
		//points = index;
		speed = glm::vec3(0.0f, 0.0f, -shipspeed);
		angle = glm::vec3(0.0f);
		radius = shipsize;
		crash = false;
		erase = false;
	}

	~Player()
	{

	}

	glm::vec3 GetMoveValue();

	void Move_by_Time();
	void Control(unsigned char key, Keyboard_type type);
	void Tilt(int x, int y);
	void Explosion();
	void DrawPlayer(GLuint shaderProgramID, GLuint transformLoc);

};