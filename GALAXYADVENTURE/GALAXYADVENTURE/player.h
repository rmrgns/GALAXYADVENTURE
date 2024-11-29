#pragma once

#include "Shape.h"

const float shipsize = 0.5f;

class Player : public Shape
{
private:

public:

	Player()
	{
		int index = 0;
		//기체 아랫면
		shapecoord[index++] = glm::vec3(-shipsize, -shipsize, shipsize);
		shapecoord[index++] = glm::vec3(shipsize, -shipsize, shipsize);
		shapecoord[index++] = glm::vec3(0, -shipsize, -shipsize);

		//기체 뒷면
		shapecoord[index++] = glm::vec3(-shipsize, -shipsize, shipsize);
		shapecoord[index++] = glm::vec3(shipsize, -shipsize, shipsize);
		shapecoord[index++] = glm::vec3(shipsize, shipsize, -shipsize);
		shapecoord[index++] = glm::vec3(-shipsize, shipsize, -shipsize);

		//기체 왼쪽
		shapecoord[index++] = glm::vec3(-shipsize, -shipsize, shipsize);
		shapecoord[index++] = glm::vec3(-shipsize, shipsize, shipsize);
		shapecoord[index++] = glm::vec3(0, shipsize, -shipsize);
		shapecoord[index++] = glm::vec3(0, -shipsize, -shipsize);

		//기체 오른쪽
		shapecoord[index++] = glm::vec3(shipsize, -shipsize, shipsize);
		shapecoord[index++] = glm::vec3(shipsize, shipsize, shipsize);
		shapecoord[index++] = glm::vec3(0, shipsize, -shipsize);
		shapecoord[index++] = glm::vec3(0, -shipsize, -shipsize);


		//기체 윗면
		shapecoord[index++] = glm::vec3(-shipsize, shipsize, shipsize);
		shapecoord[index++] = glm::vec3(shipsize, shipsize, shipsize);
		shapecoord[index++] = glm::vec3(0, shipsize, -shipsize);

		for (int i = 0; i < index; ++i)
			shapecolor[i] = glm::vec3(1.0f, 0.0f, 1.0f);

		translation = glm::vec3(0.0);
		rotation = glm::vec3(0.0);
		revolution = glm::vec3(0.0);
		scaling = glm::vec3(1.0);
		points = index;
	}

	~Player()
	{

	}

	void Move_by_Time();
	void Control(unsigned char key);
	void Tilt();
	void DrawPlayer();

};