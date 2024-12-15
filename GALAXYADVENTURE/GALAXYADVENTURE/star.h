#pragma once
#include "Shape.h"
#include "common.h"
#include "Shader.h"
#include "OBJRead.h"

class Star : public Shape
{
private:
	Model model;
	const char* filename = "OBJ/sphere1.obj";
	GLuint shaderProgramID;

	glm::vec3 pos = glm::vec3(0.f,0.f,0.f);			// position
	glm::vec3 size = glm::vec3(25.0f, 25.0f, 25.0f);		// size
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);		// color
	glm::mat4 Matrix = glm::mat4(1.f);					// calculate matrix
	glm::mat4 Transform = glm::mat4(1.f);				// transform matrix
	glm::mat4 Scale = glm::mat4(1.f);					// scale matrix

	// light
	GLfloat emissionPower = 0.5f; // power
	float lightCycle  = 0;

	// texture
	GLuint textureID; // 이 Star가 사용할 텍스처 ID

public:
	Star(GLuint shaderProgram, GLuint textureID)
		: shaderProgramID(shaderProgram), textureID(textureID) {
		model.loadFromFile(filename);

		RandomColor(color);
		RandomPosition(pos);
		size = RandomSize();

		Transform = glm::translate(Transform, pos);
		Scale = glm::scale(Scale, size);
		Matrix = Transform * Scale;
	}

	~Star() {

	}

	void Draw(GLuint transformLoc);

	void Update(float time);

	// method
	Model getModel() const { return model; }
	void setPos(glm::vec3 pos) { pos = pos; }
	glm::vec3 getPos() const { return pos; }
};

