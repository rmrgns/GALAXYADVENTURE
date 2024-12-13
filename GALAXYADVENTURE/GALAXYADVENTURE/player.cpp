#include "player.h"
#include "game.h"

glm::vec3 Player::GetMoveValue()
{
	return glm::vec3(speed.x * glm::cos(angle.y) + speed.z * glm::sin(angle.y), speed.y, -speed.x * glm::sin(angle.y) + speed.z * glm::cos(angle.y));
}

void Player::Move_by_Time()
{
	translation += GetMoveValue();
}

void Player::Control(unsigned char key, Keyboard_type type)
{
	if (type == KEY_DOWN)
	{
		switch (key)
		{
		case 'w':
			speed.y = shipspeed;
			rotation.x = glm::radians(45.0f - angle.y);
			break;
		case 's':
			speed.y = -shipspeed;
			rotation.x = glm::radians(-45.0f - angle.y);
			break;
		case 'a':
			speed.x = -shipspeed;
			rotation.z = glm::radians(45.0f + angle.y);
			break;
		case 'd':
			speed.x = shipspeed;
			rotation.z = glm::radians(-45.0f + angle.y);
			break;
		case 'r':
			speed.z = -shipspeed;
			break;
		case 'f':
			speed.z = 0.0f;
			break;
		default:
			break;
		}
	}
	else if (type == KEY_UP)
	{
		switch (key)
		{
		case 'w':
			speed.y = 0.0f;
			rotation.x = glm::radians(0.0f);
			break;
		case 's':
			speed.y = 0.0f;
			rotation.x = glm::radians(0.0f);
			break;
		case 'a':
			speed.x = 0.0f;
			rotation.z = glm::radians(0.0f);
			break;
		case 'd':
			speed.x = 0.0f;
			rotation.z = glm::radians(0.0f);
			break;
		default:
			break;
		}
	}
}

void Player::Tilt(int x, int y)
{
	angle.y -= glm::radians(float(x) / 4.0f);

	rotation.y = angle.y;
}

void Player::DrawPlayer(GLuint shaderProgramID, GLuint transformLoc)
{
	glUniform3fv(glGetUniformLocation(shaderProgramID, "objectColor"), 1, glm::value_ptr(shapecolor[0]));
	glm::mat4 matrix = glm::mat4(1.0f);
	matrix = glm::translate(matrix, translation);
	matrix = glm::rotate(matrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	matrix = glm::rotate(matrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	matrix = glm::rotate(matrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	matrix = glm::scale(matrix, scaling);
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(matrix));
	CreateModel(vao, vbo, ebo, model);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, model.faces.size() * 3, GL_UNSIGNED_INT, 0);
}

