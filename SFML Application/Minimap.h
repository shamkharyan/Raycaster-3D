#ifndef MINIMAP_H_
#define MINIMAP_H_

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Player.h"

class Minimap
{
public:
	Minimap(sf::RenderWindow& window_);
	void render(const std::vector<std::string>& map, Player& player);
private:
	void render_player(const std::vector<std::string>& map, Player& player);
	void render_map(const std::vector<std::string>& map, Player& player);
	void render_background();
private:
	sf::RenderWindow&				window;
	const std::size_t				MAP_SIZE;
	const std::size_t				BLOCK_SCALE;
	const std::size_t				BORDER_WIDTH;
	const sf::Color					WALL_COLOR;
	const sf::Color					VOID_COLOR;
	const sf::Color					GROUND_COLOR;
	const sf::Color					PLAYER_COLOR;
	const sf::Color					BORDER_COLOR;
	const sf::Color					COIN_COLOR;
	const sf::Vector2f				MAP_OFFSET;
	
};

#endif //MINIMAP_H_*/
