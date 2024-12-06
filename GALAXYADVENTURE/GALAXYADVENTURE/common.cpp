#include "common.h"

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
	uniform_real_distribution<float> random(-100.f, 100.f);
	pos.x = random(gen);
	pos.y = random(gen);
	pos.z = random(gen);
}
