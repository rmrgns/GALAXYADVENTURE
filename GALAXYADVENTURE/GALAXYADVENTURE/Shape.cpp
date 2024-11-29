#include "Shape.h"
#include "game.h"



void Shape::OldDraw(int i, GLenum mode)
{
	glDrawArrays(mode, i * MAX_POINTS, points);
}

void Shape::Draw(int i, GLenum mode)
{
	extern Game game;
	GLuint transformLoc = glGetUniformLocation(game.getShaderProgramID(), "modelTransform");

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, revolution.x, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, revolution.y, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, revolution.z, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, translation);
	model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, scaling);

	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(mode, i * MAX_POINTS, points);
}

void Shape::Draw_Without_Transform(int i, GLenum mode)
{
	glDrawArrays(mode, i * MAX_POINTS, points);
}

void Shape::DrawLineStrip(int i)
{
	glDrawArrays(GL_LINE_STRIP, i * MAX_POINTS, points);
}

void Shape::DrawCylinder(int i)
{
	int baseVertexCount = points / 3;

	// ÇÏ´Ü ¹Ø¸é
	glDrawArrays(GL_TRIANGLE_FAN, i * MAX_POINTS, baseVertexCount);

	// »ó´Ü ¹Ø¸é
	glDrawArrays(GL_TRIANGLE_FAN, i * MAX_POINTS + baseVertexCount, baseVertexCount);

	// Ãø¸é
	glDrawArrays(GL_TRIANGLE_STRIP, i * MAX_POINTS + 2 * baseVertexCount, points - 2 * baseVertexCount);

}
