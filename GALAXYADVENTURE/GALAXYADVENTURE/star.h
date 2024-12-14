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

	glm::vec3 pos = glm::vec3(0.f,1.1f,0.f);			// position
	glm::vec3 size = glm::vec3(10.0f, 10.0f, 10.0f);		// size
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);		// color
	glm::mat4 Matrix = glm::mat4(1.f);					// calculate matrix
	glm::mat4 Transform = glm::mat4(1.f);				// transform matrix
	glm::mat4 Scale = glm::mat4(1.f);					// scale matrix

	// light
	GLfloat emissionPower = 0.5f; // power
	float lightCycle  = 0;

	// texture
	int textureNum = 0;

public:
	Star(GLuint shaderProgram)
		: shaderProgramID(shaderProgram),         // shaderProgram 초기화
		textureNum(RandomTexture()),          // RandomTexture 함수로 텍스처 번호 초기화
		Transform(glm::translate(glm::mat4(1.0f), pos)), // 위치 변환 매트릭스 초기화
		Scale(glm::scale(glm::mat4(1.0f), size)),        // 크기 변환 매트릭스 초기화
		Matrix(Transform* Scale)            // 최종 변환 매트릭스 초기화
	{
		RandomColor(color);
		RandomPosition(pos);
		model.loadFromFile(filename);           // 모델 로드
	}
	~Star() {

	}

	void Draw(GLuint transformLoc);

	void Update(float time);

	// method
	Model getModel() const { return model; }
	void setPos(glm::vec3 pos) { pos = pos; }
	glm::vec3 getPos() const { return pos; }

	int getTexNum() const { return textureNum; }
};

