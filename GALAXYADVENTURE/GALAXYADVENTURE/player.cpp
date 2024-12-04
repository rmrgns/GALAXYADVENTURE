#include "player.h"

void Player::Move_by_Time()
{
	translation += speed;
}

void Player::Control(unsigned char key, Keyboard_type type)
{
	if (type == KEY_DOWN)
	{
		switch (key)
		{
		case 'w':
			speed.y = shipspeed;
			break;
		case 's':
			speed.y = -shipspeed;
			break;
		case 'a':
			speed.x = -shipspeed;
			break;
		case 'd':
			speed.x = shipspeed;
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
			break;
		case 's':
			speed.y = 0.0f;
			break;
		case 'a':
			speed.x = 0.0f;
			break;
		case 'd':
			speed.x = 0.0f;
			break;
		default:
			break;
		}
	}
}

void Player::Tilt()
{
}

void Player::DrawPlayer()
{
	Draw(0, GL_TRIANGLES);
}
