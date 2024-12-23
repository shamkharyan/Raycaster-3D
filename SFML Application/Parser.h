#ifndef PARSER_H_
#define PARSER_H_

#include "Settings.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Parser
{
public:
	enum class Error { SUCCESS, NO_MAP, NO_TEXTURES, BAD_SYMBOL, BAD_BORDER, NO_PATH, NO_PLAYER, NO_COIN, MANY_PLAYERS };
	Parser(Settings& settings_);
	Error check_all();
	sf::Vector2u get_player() const;
	std::vector<sf::Vector2f> get_coins() const;
	std::vector<std::string> get_map() const;
	Error get_state() const;
	void set_state();
	bool check_existing(std::string path);
private:
	Error parse_map();
	Error check_files();
	Error check_borders();
	Error check_content();
	Error check_paths();
	void flood_fill(std::vector<std::string>& copy_map, sf::Vector2u coord, std::size_t& c);
private:
	Settings& settings;
	std::vector<std::string> map;
	sf::Vector2u player;
	std::vector<sf::Vector2f> coins;
	Error state;
};

#endif //PARSER_H_
