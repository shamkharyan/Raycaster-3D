#ifndef GAMEPLAY_H_
#define GAMEPLAY_H_

#include "Player.h"
#include "Parser.h"
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "Minimap.h"
#include "Settings.h"
#include <vector>
#include <string>
#include "Minimap.h"
#include "Page.h"

class Gameplay
{
public:
	Gameplay(sf::RenderWindow& window_, Settings& settings_, Parser& parser_);
	void render();
	void handle_events(sf::Event& event, Page& current, bool& game_started);
	void init();
	void update();
private:
	void clear();
	void move_forward();
	void move_backward();
	void move_left();
	void move_right();
	void rotate_mouse();

	void render_ground();
	void render_coin(const sf::Vector2f& spritePos, const std::vector<float>& z_buffer);
	void render_score();
	sf::Vector2f calculate_coin_distance(const sf::Vector2f& a);
private:
	std::vector<std::string> map;
	sf::RenderWindow& window;
	std::vector<sf::Vector2f> coins;
	std::size_t score;
	std::size_t max_score;
	Player player;
	Minimap minimap;
	Settings& settings;
	Parser& parser;
	sf::Texture NO_texture;
	sf::Texture SO_texture;
	sf::Texture EA_texture;
	sf::Texture WE_texture;
	sf::Texture coin_texture;
	sf::Font	font;
	sf::Clock	clk;
	sf::Clock	dt;
	sf::SoundBuffer coin_sound_buffer;
	sf::Sound		coin_sound;
	sf::SoundBuffer win_sound_buffer;
	sf::Sound		win_sound;
};

#endif //GAMEPLAY_H_
