#ifndef PLAYER_H_
#define PLAYER_H_

#include <SFML/Graphics.hpp>

struct Player
{
	sf::Vector2f	pos;
	sf::Vector2f	dir;
	sf::Vector2f	plane;
	float			base_mov_speed;
	float			mov_speed;
	float			rot_speed;
	bool			up_enabled;
	bool			down_enabled;
	bool			left_enabled;
	bool			right_enabled;
	bool			sprint;
};

#endif //PLAYER_H_
