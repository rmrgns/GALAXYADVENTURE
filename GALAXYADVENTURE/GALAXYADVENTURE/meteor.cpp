#include "meteor.h"
#include "game.h"

Meteor::Meteor()
{
	model.loadFromFile(filename);
	CreateModel(vaoMeteor, vboMeteor, eboMeteor, model);
	RandomColor(color);
	RandomPosition(pos);
	Transform = glm::translate(Transform, pos);
	Scale = glm::scale(Scale, size);
	Rotate = glm::rotate(Rotate, glm::radians(rotate.y), glm::vec3(0.f, 1.f, 0.f));
	Matrix = Transform * Rotate * Scale;
	radius = size.x;
}

void Meteor::Draw(GLuint shaderProgramID, GLuint transformLoc) const
{
	// emission effect
	glUniform1f(glGetUniformLocation(shaderProgramID, "emissionPower"), 0.5);

	// matrix
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(Matrix));

	// render
	
	glBindVertexArray(vaoMeteor);
	glDrawElements(GL_TRIANGLES, model.faces.size() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
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
