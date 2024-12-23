#ifndef PLAYER_H_
#define PLAYER_H_

#include <SFML/Graphics.hpp>

struct Player
{
	sf::Vector2f	pos;
	sf::Vector2f	dir;
	sf::Vector2f	plane;
	float			mov_speed;
	float			rot_speed;
};

#endif //PLAYER_H_
