#pragma once

#include "Shape.h"
#include "common.h"
#include "Shader.h"
#include "OBJRead.h"

class Star : public Shape
{
private:
	Model star;
	const char* filename = "cube_sample.obj";
public:
	Star() {
		read_obj_file(filename, &star);
	}
	~Star() {

	}
	Model getModel() const { return star; }
};

