#include "common.h"

//Bounding Sphere 방식 충돌 검사
bool CheckColide_BS(glm::vec3 sphere1, glm::vec3 sphere2, float radius1, float radius2)
{
	return glm::sqrt(glm::pow(sphere2.x - sphere1.x, 2.0f) + glm::pow(sphere2.y - sphere1.y, 2.0f) + glm::pow(sphere2.z - sphere1.z, 2.0f)) <= radius1 + radius2;
}

void RandomColor(float& colorR, float& colorG, float& colorB)
{
	uniform_real_distribution<float> random(0, 1);
	colorR = random(gen);
	colorG = random(gen);
	colorB = random(gen);
}

void RandomColor(glm::vec3 &color)
{
	uniform_real_distribution<float> random(0, 1);
	color.r = random(gen);
	color.g = random(gen);
	color.b = random(gen);
}

void RandomPosition(glm::vec3& pos)
{
	uniform_real_distribution<float> random(-10.f, 10.f);
	pos.x = random(gen);
	pos.y = random(gen);
	pos.z = random(gen);
}
