#ifndef MENU_H_
#define MENU_H_

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Settings.h"
#include "Parser.h"
#include <vector>
#include "Page.h"

class Menu
{
public:
	Menu(sf::RenderWindow& window_, Settings& settings_, Parser& parser_);
	void render_menu();
	void render_settings();
	void handle_menu_events(sf::Event& event, Page& current);
	void handle_settings_events(sf::Event& event, Page& current);
private:
	enum class Buttons
	{
		START_GAME,
		SETTINGS,
		EXIT, 
		MAP_PATH, 
		MINIMAP,
		NO_TEXTURE, 
		SO_TEXTURE,
		EA_TEXTURE,
		WE_TEXTURE,
		COIN_TEXTURE,
		MOVE_SPEED, 
		ROT_SPEED, 
		SKY_COLOR, 
		GROUND_COLOR, 
		FOV,
		BACK, 
		RESET,
		SOUND
	};

	void render_label(const char* title, sf::Vector2f pos, std::size_t size);
	void render_button(const char* title, sf::Vector2f pos, sf::Vector2f size, Buttons type, sf::Color outline);
	void render_background();
	void render_menu_buttons();
	void render_settings_buttons();
	void render_settings_files();
	void render_settings_options();

	const char* get_option_text(Settings::Option op) const;
	const char* get_color_text(Settings::Colors color) const;
	const char* get_error_text(Parser::Error err) const;
private:
	sf::RenderWindow&				window;
	sf::Texture						background_texture;
	sf::Font						font;
	sf::Texture						button_texture;
	std::vector<std::pair<Buttons,
		sf::RectangleShape>>		buttons;
	std::vector<sf::Text>			texts;
	Settings&						settings;
	Parser&							parser;
	sf::SoundBuffer					click_buffer;
	sf::Sound						click;
};

#endif //MENU_H_
