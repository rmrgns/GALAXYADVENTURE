#include "meteor.h"
#include "game.h"

void Meteor::Draw(GLuint shaderProgramID, GLuint transformLoc) const
{
	glUniform1f(glGetUniformLocation(shaderProgramID, "emissionPower"), 0.5);
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(Matrix));
	CreateModel(vaoStar, vboStar, eboStar, model);
	glBindVertexArray(vaoStar);
	glDrawElements(GL_TRIANGLES, model.faces.size() * 3, GL_UNSIGNED_INT, 0);
}

void Meteor::Update(float time)
{
	m_time += time;
	if (m_time >= 1)
	{
		rotate.y += 1;
		Rotate = glm::rotate(Rotate, glm::radians(rotate.y), glm::vec3(0.f, 1.f, 0.f));
		Matrix = Transform * Rotate * Scale;
		m_time = 0.f;
	}
}
