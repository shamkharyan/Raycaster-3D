#include "Parser.h"
#include "Settings.h"
#include <vector>
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>

Parser::Parser(Settings& settings_) : settings(settings_), coins(0), state(Parser::Error::NO_MAP) {}

bool Parser::check_existing(std::string path)
{
	std::ifstream file(path);
	if (!file.good())
		return false;
	file.close();
	return true;
}

Parser::Error Parser::parse_map()
{
	map.clear();
	coins.clear();
	std::string line;
	std::ifstream file(settings.get_path());
	if (!file.good())
		return Parser::Error::NO_MAP;
	while (std::getline(file, line))
	{
		if (line.find_first_not_of(" \t") == std::string::npos)
			continue;
		if (std::strspn(line.c_str(), ".1CcPp ") != line.length())
			return Parser::Error::BAD_SYMBOL;
		map.push_back(line);
		
	}
	file.close();
	return Parser::Error::SUCCESS;
}

Parser::Error Parser::check_files()
{
	if (!check_existing(settings.get_path()))
		return Error::NO_MAP;
	if (!check_existing(settings.get_NO_texture()) ||
		!check_existing(settings.get_SO_texture()) ||
		!check_existing(settings.get_EA_texture()) ||
		!check_existing(settings.get_WE_texture()) ||
		!check_existing(settings.get_coin_texture()))
		return Error::NO_TEXTURES;
	return Error::SUCCESS;
}

Parser::Error Parser::check_borders()
{
	std::size_t height = map.size();
	for (std::size_t i = 0; i < height; ++i)
	{
		std::size_t width = map[i].size();
		for (std::size_t j = 0; j < width; ++j)
		{
			if (tolower(map[i][j]) == 'p' || tolower(map[i][j]) == 'c' || map[i][j] == '.')
			{
				if (i == 0 || i == height - 1 || j == 0 || j == width - 1 ||
					map[i + 1][j] == ' ' || map[i - 1][j] == ' ' ||
					map[i][j + 1] == ' ' || map[i][j - 1] == ' ')
					return Parser::Error::BAD_BORDER;
			}
		}
	}
	return Parser::Error::SUCCESS;
}

Parser::Error Parser::check_content()
{
	bool player_found = false;

	std::size_t height = map.size();
	for (std::size_t i = 0; i < height; ++i)
	{
		std::size_t width = map[i].size();
		for (std::size_t j = 0; j < width; ++j)
		{
			if (tolower(map[i][j]) == 'c')
				coins.push_back(sf::Vector2f(j, i));
			else if (!player_found && tolower(map[i][j]) == 'p')
			{
				player_found = true;
				player.x = j;
				player.y = i;
			}
			else if (player_found && tolower(map[i][j]) == 'p')
				return Parser::Error::MANY_PLAYERS;
		}
	}
	if (coins.size() == 0)
		return Parser::Error::NO_COIN;
	if (!player_found)
		return Parser::Error::NO_PLAYER;
	return Parser::Error::SUCCESS;
}

void Parser::flood_fill(std::vector<std::string>& copy_map, sf::Vector2u coord, std::size_t& c)
{
	if (copy_map[coord.y][coord.x] == '1')
		return;
	if (tolower(copy_map[coord.y][coord.x]) == 'c')
		--c;
	copy_map[coord.y][coord.x] = '1';
	flood_fill(copy_map, sf::Vector2u(coord.x, coord.y + 1), c);
	flood_fill(copy_map, sf::Vector2u(coord.x, coord.y - 1), c);
	flood_fill(copy_map, sf::Vector2u(coord.x + 1, coord.y), c);
	flood_fill(copy_map, sf::Vector2u(coord.x - 1, coord.y), c);
}

Parser::Error Parser::check_paths()
{
	std::size_t c = coins.size();
	std::vector<std::string> copy_map(map);
	copy_map[player.y][player.x] = '.';
	flood_fill(copy_map, player, c);
	if (c != 0)
		return Parser::Error::NO_PATH;
	return Parser::Error::SUCCESS;
}

Parser::Error Parser::check_all()
{
	Parser::Error err;
	err = check_files();
	if (err != Error::SUCCESS)
		return err;
	err = parse_map();
	if (err != Error::SUCCESS)
		return err;
	err = check_borders();
	if (err != Error::SUCCESS)
		return err;
	err = check_content();
	if (err != Error::SUCCESS)
		return err;
	err = check_paths();
	if (err != Error::SUCCESS)
		return err;
	return err;
}

sf::Vector2u Parser::get_player() const { return player; }
std::vector<sf::Vector2f> Parser::get_coins() const { return coins; }
std::vector<std::string> Parser::get_map() const { return map; }
Parser::Error Parser::get_state() const { return state; }
void Parser::set_state() { state = check_all(); }