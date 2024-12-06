#pragma once

#include "Shape.h"
#include "common.h"
#include "Shader.h"
#include "OBJRead.h"

class Star : public Shape
{
private:
	Model star;
	const char* filename = "OBJ/sphere.obj";
	glm::vec3 pos = glm::vec3(0.f,0.f,0.f);
public:
	Star() {
		star.loadFromFile(filename);
	}
	~Star() {

	}



	void Update();

	// method
	Model getModel() const { return star; }
};

