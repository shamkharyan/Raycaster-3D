#include "Minimap.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>

Minimap::Minimap(sf::RenderWindow& window_) :
	window(window_),
	MAP_SIZE(200),
	BLOCK_SCALE(10),
	BORDER_WIDTH(5),
	WALL_COLOR(sf::Color(0x88, 0x88, 0x88)),
	VOID_COLOR(sf::Color::Black),
	PLAYER_COLOR(sf::Color::Red),
	BORDER_COLOR(sf::Color::Red),
	GROUND_COLOR(sf::Color::White),
	COIN_COLOR(sf::Color::Green),
	MAP_OFFSET(sf::Vector2f(15, 15)) {}

void Minimap::render_background()
{
	sf::RectangleShape background;
	background.setFillColor(VOID_COLOR);
	background.setPosition(MAP_OFFSET);
	background.setSize(sf::Vector2f(MAP_SIZE, MAP_SIZE));
	background.setOutlineThickness(BORDER_WIDTH);
	background.setOutlineColor(BORDER_COLOR);
	window.draw(background);
}

void Minimap::render_map(const std::vector<std::string>& map, Player& player)
{
	std::size_t map_height = map.size();
	std::vector<sf::RectangleShape> recs;

	for (std::size_t y = 0; y < map_height; ++y)
	{
		std::size_t map_width = map[y].size();
		for (std::size_t x = 0; x < map_width; ++x)
		{
			sf::Vector2f rect_pos(
				MAP_OFFSET.x + (x - player.pos.x) * BLOCK_SCALE + MAP_SIZE / 2.f,
				MAP_OFFSET.y + (y - player.pos.y) * BLOCK_SCALE + MAP_SIZE / 2.f
			);

			//Because positioning starts from offset, end position must be decreased by 2 * border_width
			if (rect_pos.x >= MAP_OFFSET.x && rect_pos.x < MAP_OFFSET.x + MAP_SIZE - 2 * BORDER_WIDTH &&
				rect_pos.y >= MAP_OFFSET.y && rect_pos.y < MAP_OFFSET.y + MAP_SIZE - 2 * BORDER_WIDTH)
			{
				if (map[y][x] == '1' || map[y][x] == '0' || tolower(map[y][x]) == 'c')
				{
					recs.push_back(sf::RectangleShape());
					recs.back().setSize(sf::Vector2f(BLOCK_SCALE, BLOCK_SCALE));
					recs.back().setPosition(rect_pos);
					if (map[y][x] == '1')
						recs.back().setFillColor(WALL_COLOR);
					else if (tolower(map[y][x]) == 'c')
						recs.back().setFillColor(COIN_COLOR);
					else
						recs.back().setFillColor(GROUND_COLOR);
					window.draw(recs.back());
				}
			}
		}
	}
}

void Minimap::render_player(const std::vector<std::string>& map, Player& player)
{
	sf::Vector2f player_pos(
		MAP_OFFSET.x + (MAP_SIZE / 2.f) - (BLOCK_SCALE / 2.f),
		MAP_OFFSET.y + (MAP_SIZE / 2.f) - (BLOCK_SCALE / 2.f)
	);

	sf::CircleShape point(BLOCK_SCALE / 2.f);
	point.setFillColor(PLAYER_COLOR);
	point.setPosition(player_pos);
	window.draw(point);

}

void Minimap::render(const std::vector<std::string>& map, Player& player)
{
	render_background();
	render_map(map, player);
	render_player(map, player);
}
