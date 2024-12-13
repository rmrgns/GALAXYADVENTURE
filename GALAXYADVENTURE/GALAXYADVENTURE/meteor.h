#pragma once
#include "Shape.h"
#include "common.h"
#include "Shader.h"
#include "OBJRead.h"

class Meteor : public Shape
{
private:
	Model model;
	const char* filename = "OBJ/sphere.obj";

	glm::vec3 pos = glm::vec3(0.f, 1.1f, 0.f);			// position
	glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);	// color
	glm::mat4 Matrix = glm::mat4(1.f);				// calculate matrix
	glm::mat4 Transform = glm::mat4(1.f);			// transform matrix
public:
	Meteor()
	{
		model.loadFromFile(filename);

		RandomColor(color);
		RandomPosition(pos);
		Transform = glm::translate(Transform, pos);
		Matrix = Transform;
	}

	~Meteor()
	{

	}

	void Draw(GLuint shaderProgramID, GLuint transformLoc) const;

	void Update();

	// method
	Model getModel() const { return model; }
	void setPos(glm::vec3 pos) { pos = pos; }
	glm::vec3 getPos() const { return pos; }

};