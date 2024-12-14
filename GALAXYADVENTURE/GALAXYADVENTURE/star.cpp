#include "star.h"
#include "game.h"


void Star::Draw(GLuint shaderProgramID, GLuint transformLoc)
{
	glUniform1f(glGetUniformLocation(shaderProgramID, "emissionPower"), emissionPower);
	glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, glm::value_ptr(color));
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(Matrix));

	//GLuint lightColorLoc = glGetUniformLocation(shaderProgramID, "lightColor");
	//GLuint emissionPowerLoc = glGetUniformLocation(shaderProgramID, "emissionPower");
	//glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
	//glUniform1f(emissionPowerLoc, emissionPower);
	

	CreateModel(vaoStar, vboStar, eboStar, model);

	glBindVertexArray(vaoStar);
	glDrawElements(GL_TRIANGLES, model.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Star::Update(float time)
{
	lightCycle += time;
	emissionPower = 0.75f + 0.25f * sin(lightCycle); // 0.5 ~ 1.0 사이로 변동
}
