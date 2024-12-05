#include "player.h"

glm::vec3 Player::GetMoveValue()
{
	return glm::vec3(speed.x + speed.z * glm::sin(angle.y), speed.y, speed.z * glm::cos(angle.y));
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
			rotation.x = glm::radians(45.0f);
			break;
		case 's':
			speed.y = -shipspeed;
			rotation.x = glm::radians(-45.0f);
			break;
		case 'a':
			speed.x = -shipspeed;
			rotation.z = glm::radians(45.0f);
			break;
		case 'd':
			speed.x = shipspeed;
			rotation.z = glm::radians(-45.0f);
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
	const int tiltstandard = 100;

	if (x > tiltstandard)
		angle.y -= glm::radians(1.0f);
	else if (x < -tiltstandard)
		angle.y += glm::radians(1.0f);

	rotation.y = angle.y;
}

void Player::DrawPlayer()
{
	Draw(0, GL_TRIANGLES);
}
