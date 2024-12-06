#include "star.h"
#include "game.h"

void Star::Draw(GLuint shaderProgramID, GLuint transformLoc) const
{
	glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, glm::value_ptr(color));
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(Matrix));
	CreateModel(vao, vbo, ebo, model);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, model.faces.size() * 3, GL_UNSIGNED_INT, 0);
}

void Star::Update()
{
}
