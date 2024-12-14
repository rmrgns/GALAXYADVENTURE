#include "meteor.h"
#include "game.h"

void Meteor::Draw(GLuint shaderProgramID, GLuint transformLoc) const
{
	glUniform1f(glGetUniformLocation(shaderProgramID, "emissionPower"), 0.5);
	glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, glm::value_ptr(color));
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(Matrix));
	CreateModel(vaoStar, vboStar, eboStar, model);
	glBindVertexArray(vaoStar);
	glDrawElements(GL_TRIANGLES, model.faces.size() * 3, GL_UNSIGNED_INT, 0);
}

void Meteor::Update()
{
}
