#include "Menu.h"
#include <vector>
#include <utility>
#include <iostream>
#include "Minimap.h"
#include "Gameplay.h"
#include "Page.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

Menu::Menu(sf::RenderWindow& window_, Settings& settings_, Parser& parser_) : 
	window(window_), settings(settings_), parser(parser_)
{
	if (!font.loadFromFile("..\\Resources\\Fonts\\Silkscreen.ttf"))
		font.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf");
	background_texture.loadFromFile("..\\Resources\\Menu Textures\\stars.png");
	button_texture.loadFromFile("..\\Resources\\Menu Textures\\button_background.png");
	click_buffer.loadFromFile("..\\Resources\\Sounds\\click.mp3");
	click.setBuffer(click_buffer);
	parser.set_state();
}

void Menu::render_menu()
{
	const sf::Vector2f WINDOW_SIZE = static_cast<sf::Vector2f>(window.getSize());
	const std::size_t TITLE_SIZE = 78;
	const sf::Vector2f TITLE_POSITION(WINDOW_SIZE.x / 2.f, WINDOW_SIZE.y * 0.15f);
	const sf::Vector2f LABEL_POSITION(WINDOW_SIZE.x / 2.f, WINDOW_SIZE.y * 0.27f);

	buttons.clear();
	texts.clear();
	render_background();
	render_label("RAYCASTER 3D", TITLE_POSITION, TITLE_SIZE);
	render_label(get_error_text(parser.get_state()), LABEL_POSITION, TITLE_SIZE / 3);
	render_menu_buttons();
}

void Menu::render_settings()
{
	buttons.clear();
	texts.clear();
	render_background();
	render_settings_buttons();
	render_settings_files();
	render_settings_options();
}

void Menu::render_background()
{
	const std::size_t R = 0x35;
	const std::size_t G = 0x06;
	const std::size_t B = 0x3E;

	sf::RectangleShape background(static_cast<sf::Vector2f>(window.getSize()));
	window.clear(sf::Color(R, G, B));
	if (background.getSize().x > 0 && background.getSize().y > 0)
	{
		background_texture.setRepeated(true);
		background.setTexture(&background_texture);
		background.setTextureRect(sf::IntRect(0, 0, background.getSize().x, background.getSize().y));
		window.draw(background);
	}
}

void Menu::render_label(const char* title, sf::Vector2f pos, std::size_t size)
{
	const std::size_t	OUTLINE_THICKNESS = 8;

	sf::Text name(title, font, size);
	sf::FloatRect text_bounds = name.getLocalBounds();
	name.setOrigin(text_bounds.width / 2.f, text_bounds.height / 2.f);
	name.setPosition(pos);
	name.setOutlineThickness(OUTLINE_THICKNESS);
	window.draw(name);
}

void Menu::render_menu_buttons()
{
	const sf::Vector2f	WINDOW_SIZE = static_cast<sf::Vector2f>(window.getSize());
	const sf::Vector2f	BUTTON_SIZE(375, 75);
	const std::size_t	WINDOW_CENTER_X = WINDOW_SIZE.x / 2.f;

	render_button(
		"Start Game",
		sf::Vector2f(WINDOW_CENTER_X, WINDOW_SIZE.y * 0.4f),
		BUTTON_SIZE,
		Buttons::START_GAME,
		sf::Color::White
	);
	render_button(
		"Settings",
		sf::Vector2f(WINDOW_CENTER_X, WINDOW_SIZE.y * 0.6f),
		BUTTON_SIZE,
		Buttons::SETTINGS,
		sf::Color::White
	);
	render_button(
		"Exit",
		sf::Vector2f(WINDOW_CENTER_X, WINDOW_SIZE.y * 0.8f),
		BUTTON_SIZE,
		Buttons::EXIT,
		sf::Color::White
	);
}

void Menu::render_button(const char* title, sf::Vector2f pos, sf::Vector2f size, Buttons type, sf::Color outline)
{
	const std::size_t BUTTON_THICKNESS = 5;
	const std::size_t BUTTON_TEXT_SIZE = 28;
	const std::size_t BUTTON_TEXT_THICKNESS = 2;

	sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);

	sf::RectangleShape button(size);
	if (button_texture.getSize().x > 0 && button_texture.getSize().y > 0)
	{
		button.setTexture(&button_texture);
		button.setTextureRect(sf::IntRect(0, 0, size.x, size.y));
	}
	else
		button.setFillColor(sf::Color(0xFF, 0x4D, 0));
	button.setOutlineThickness(BUTTON_THICKNESS);
	sf::FloatRect button_lbounds = button.getLocalBounds();
	button.setOrigin(
		button_lbounds.width / 2.f,
		button_lbounds.height / 2.f
	);
	button.setPosition(pos.x, pos.y);
	button.setOutlineColor(sf::Color::Black);

	sf::Text text(title, font, BUTTON_TEXT_SIZE);
	text.setFillColor(sf::Color::Black);
	text.setStyle(sf::Text::Bold);
	text.setOutlineColor(outline);
	text.setOutlineThickness(BUTTON_TEXT_THICKNESS);
	sf::FloatRect text_bounds = text.getLocalBounds();
	sf::FloatRect button_rounds = button.getGlobalBounds();
	text.setOrigin(
		text_bounds.left + text_bounds.width / 2.f,
		text_bounds.top + text_bounds.height / 2.f
	);
	text.setPosition(
		button_rounds.left + button_rounds.width / 2.f,
		button_rounds.top + button_rounds.height / 2.f
	);
	buttons.push_back(std::pair<Buttons, sf::RectangleShape>(type, button));
	texts.push_back(text);
	if (button.getGlobalBounds().contains(sf::Vector2f(mouse_pos.x, mouse_pos.y)))
	{
		button.setScale(1.15f, 1.15f);
		text.setScale(1.15f, 1.15f);
	}
	window.draw(button);
	window.draw(text);
}

void Menu::handle_menu_events(sf::Event& event, Page& current)
{
	if (event.type == sf::Event::EventType::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
		for (std::size_t i = 0; i < buttons.size(); ++i)
		{
			if (buttons[i].second.getGlobalBounds().contains(sf::Vector2f(mouse_pos.x, mouse_pos.y)))
			{
				if (settings.get_sound())
					click.play();
				switch (buttons[i].first)
				{
				case Buttons::START_GAME:
					if (parser.get_state() == Parser::Error::SUCCESS)
						current = Page::GAMEPLAY;
					break;
				case Buttons::SETTINGS:
					current = Page::MENU_SETTINGS;
					break;
				case Buttons::EXIT:
					window.close();
				}
				break;
			}
		}
	}
}

void Menu::handle_settings_events(sf::Event& event, Page& current)
{
	if (event.type == sf::Event::EventType::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
		for (std::size_t i = 0; i < buttons.size(); ++i)
		{
			if (buttons[i].second.getGlobalBounds().contains(sf::Vector2f(mouse_pos.x, mouse_pos.y)))
			{
				if (settings.get_sound())
					click.play();
				switch (buttons[i].first)
				{
				case Buttons::SOUND:
					settings.switch_sound();
					break;
				case Buttons::BACK:
					current = Page::MENU_MAIN;
					break;
				case Buttons::MINIMAP:
					settings.switch_minimap();
					break;
				case Buttons::FOV:
					settings.incr_fov();
					break;
				case Buttons::SKY_COLOR:
					settings.incr_sky_color();
					break;
				case Buttons::GROUND_COLOR:
					settings.incr_ground_color();
					break;
				case Buttons::MOVE_SPEED:
					settings.incr_mov_speed();
					break;
				case Buttons::ROT_SPEED:
					settings.incr_rot_speed();
					break;
				case Buttons::RESET:
					settings.set_default();
					parser.set_state();
					break;
				case Buttons::MAP_PATH:
					settings.set_path();
					parser.set_state();
					break;
				case Buttons::NO_TEXTURE:
					settings.set_NO_texture();
					parser.set_state();
					break;
				case Buttons::SO_TEXTURE:
					settings.set_SO_texture();
					parser.set_state();
					break;
				case Buttons::EA_TEXTURE:
					settings.set_EA_texture();
					parser.set_state();
					break;
				case Buttons::WE_TEXTURE:
					settings.set_WE_texture();
					parser.set_state();
					break;
				case Buttons::COIN_TEXTURE:
					settings.set_coin_texture();
					parser.set_state();
					break;
				}
				break;
			}
		}
	}
}

void Menu::render_settings_buttons()
{
	const sf::Vector2f	WINDOW_SIZE = static_cast<sf::Vector2f>(window.getSize());
	const sf::Vector2f	BUTTON_SIZE(250, 50);
	const std::size_t	BUTTON_Y_POS = WINDOW_SIZE.y * 0.92f;

	render_button(
		"Back",
		sf::Vector2f(WINDOW_SIZE.x * 0.2f, BUTTON_Y_POS),
		BUTTON_SIZE,
		Buttons::BACK,
		sf::Color::White
	);
	render_button(
		"Reset",
		sf::Vector2f(WINDOW_SIZE.x * 0.5f, BUTTON_Y_POS),
		BUTTON_SIZE,
		Buttons::RESET,
		sf::Color::White
	);
	render_button(
		settings.get_sound()? "Sound: ON" : "Sound: OFF",
		sf::Vector2f(WINDOW_SIZE.x * 0.8f, BUTTON_Y_POS),
		BUTTON_SIZE,
		Buttons::SOUND,
		sf::Color::White
	);
}

void Menu::render_settings_files()
{
	const sf::Vector2f	WINDOW_SIZE = static_cast<sf::Vector2f>(window.getSize());
	const sf::Vector2f	BUTTON_SIZE(200, 40);
	const std::size_t	LABEL_LEFT_X = WINDOW_SIZE.x * 0.125f;
	const std::size_t	BUTTON_LEFT_X = WINDOW_SIZE.x * 0.350f;
	const std::size_t	LABEL_RIGHT_X = WINDOW_SIZE.x * 0.6f;
	const std::size_t	BUTTON_RIGHT_X = WINDOW_SIZE.x * 0.825f;
	const std::size_t	LABEL_SIZE = 32;
	float				y_pos = 0.08f;

	render_label(
		"Files",
		sf::Vector2f(WINDOW_SIZE.x / 2.f, WINDOW_SIZE.y * y_pos),
		LABEL_SIZE * 1.5f
	);

	y_pos += 0.1f;
	render_label(
		"Map :",
		sf::Vector2f(LABEL_LEFT_X, WINDOW_SIZE.y * y_pos),
		LABEL_SIZE
	);
	render_button(
		!parser.check_existing(settings.get_path())? "Select..." : "Selected",
		sf::Vector2f(BUTTON_LEFT_X, WINDOW_SIZE.y * (y_pos + 0.02f)),
		BUTTON_SIZE,
		Buttons::MAP_PATH,
		!parser.check_existing(settings.get_path())? sf::Color::Red : sf::Color::Green
	);
	render_label(
		"Coin :",
		sf::Vector2f(LABEL_RIGHT_X, WINDOW_SIZE.y * y_pos),
		LABEL_SIZE
	);
	render_button(
		!parser.check_existing(settings.get_coin_texture())? "Select..." : "Selected",
		sf::Vector2f(BUTTON_RIGHT_X, WINDOW_SIZE.y * (y_pos + 0.02f)),
		BUTTON_SIZE,
		Buttons::COIN_TEXTURE,
		!parser.check_existing(settings.get_coin_texture())? sf::Color::Red : sf::Color::Green
	);

	y_pos += 0.1f;
	render_label(
		"North :",
		sf::Vector2f(LABEL_LEFT_X, WINDOW_SIZE.y * y_pos),
		LABEL_SIZE
	);
	render_button(
		!parser.check_existing(settings.get_NO_texture()) ? "Select..." : "Selected",
		sf::Vector2f(BUTTON_LEFT_X, WINDOW_SIZE.y * (y_pos + 0.02f)),
		BUTTON_SIZE,
		Buttons::NO_TEXTURE,
		!parser.check_existing(settings.get_NO_texture()) ? sf::Color::Red : sf::Color::Green
	);
	render_label(
		"South :",
		sf::Vector2f(LABEL_RIGHT_X, WINDOW_SIZE.y * y_pos),
		LABEL_SIZE
	);
	render_button(
		!parser.check_existing(settings.get_SO_texture()) ? "Select..." : "Selected",
		sf::Vector2f(BUTTON_RIGHT_X, WINDOW_SIZE.y * (y_pos + 0.02f)),
		BUTTON_SIZE,
		Buttons::SO_TEXTURE,
		!parser.check_existing(settings.get_SO_texture()) ? sf::Color::Red : sf::Color::Green
	);

	y_pos += 0.1f;
	render_label(
		"East :",
		sf::Vector2f(LABEL_LEFT_X, WINDOW_SIZE.y * y_pos),
		LABEL_SIZE
	);
	render_button(
		!parser.check_existing(settings.get_EA_texture()) ? "Select..." : "Selected",
		sf::Vector2f(BUTTON_LEFT_X, WINDOW_SIZE.y * (y_pos + 0.02f)),
		BUTTON_SIZE,
		Buttons::EA_TEXTURE,
		!parser.check_existing(settings.get_EA_texture()) ? sf::Color::Red : sf::Color::Green
	);
	render_label(
		"West :",
		sf::Vector2f(LABEL_RIGHT_X, WINDOW_SIZE.y * y_pos),
		LABEL_SIZE
	);
	render_button(
		!parser.check_existing(settings.get_WE_texture()) ? "Select..." : "Selected",
		sf::Vector2f(BUTTON_RIGHT_X, WINDOW_SIZE.y * (y_pos + 0.02f)),
		BUTTON_SIZE,
		Buttons::WE_TEXTURE,
		!parser.check_existing(settings.get_WE_texture()) ? sf::Color::Red : sf::Color::Green
	);
}

void Menu::render_settings_options()
{
	const sf::Vector2f	WINDOW_SIZE = static_cast<sf::Vector2f>(window.getSize());
	const sf::Vector2f	BUTTON_SIZE(200, 40);
	const std::size_t	LABEL_LEFT_X = WINDOW_SIZE.x * 0.125f;
	const std::size_t	BUTTON_LEFT_X = WINDOW_SIZE.x * 0.350f;
	const std::size_t	LABEL_RIGHT_X = WINDOW_SIZE.x * 0.6f;
	const std::size_t	BUTTON_RIGHT_X = WINDOW_SIZE.x * 0.825f;
	const std::size_t	LABEL_SIZE = 32;
	float				y_pos = 0.48f;

	render_label(
		"Options",
		sf::Vector2f(WINDOW_SIZE.x / 2.f, WINDOW_SIZE.y * y_pos),
		LABEL_SIZE * 1.5f
	);

	y_pos += 0.1f;
	render_label(
		"Minimap :",
		sf::Vector2f(LABEL_LEFT_X, WINDOW_SIZE.y * y_pos),
		LABEL_SIZE
	);
	render_button(
		settings.get_minimap()? "Enabled" : "Disabled",
		sf::Vector2f(BUTTON_LEFT_X, WINDOW_SIZE.y * (y_pos + 0.02f)),
		BUTTON_SIZE,
		Buttons::MINIMAP,
		sf::Color::White
	);
	render_label(
		"FOV :",
		sf::Vector2f(LABEL_RIGHT_X, WINDOW_SIZE.y * y_pos),
		LABEL_SIZE
	);
	render_button(
		get_option_text(settings.get_fov()),
		sf::Vector2f(BUTTON_RIGHT_X, WINDOW_SIZE.y * 0.6f),
		BUTTON_SIZE,
		Buttons::FOV,
		sf::Color::White
	);

	y_pos += 0.1f;
	render_label(
		"Ground :",
		sf::Vector2f(LABEL_LEFT_X, WINDOW_SIZE.y * y_pos),
		LABEL_SIZE
	);
	render_button(
		get_color_text(settings.get_ground_color()),
		sf::Vector2f(BUTTON_LEFT_X, WINDOW_SIZE.y * 0.7f),
		BUTTON_SIZE,
		Buttons::GROUND_COLOR,
		settings.get_color(settings.get_ground_color())
	);

	render_label(
		"Sky :",
		sf::Vector2f(LABEL_RIGHT_X, WINDOW_SIZE.y * y_pos),
		LABEL_SIZE
	);
	render_button(
		get_color_text(settings.get_sky_color()),
		sf::Vector2f(BUTTON_RIGHT_X, WINDOW_SIZE.y * 0.7f),
		BUTTON_SIZE,
		Buttons::SKY_COLOR,
		settings.get_color(settings.get_sky_color())
	);

	y_pos += 0.1f;
	render_label(
		"Mov speed :",
		sf::Vector2f(LABEL_LEFT_X, WINDOW_SIZE.y * y_pos),
		LABEL_SIZE
	);
	render_button(
		get_option_text(settings.get_mov_speed()),
		sf::Vector2f(BUTTON_LEFT_X, WINDOW_SIZE.y * 0.8f),
		BUTTON_SIZE,
		Buttons::MOVE_SPEED,
		sf::Color::White
	);
	render_label(
		"Rot speed :",
		sf::Vector2f(LABEL_RIGHT_X, WINDOW_SIZE.y * y_pos),
		LABEL_SIZE
	);
	render_button(
		get_option_text(settings.get_rot_speed()),
		sf::Vector2f(BUTTON_RIGHT_X, WINDOW_SIZE.y * 0.8f),
		BUTTON_SIZE,
		Buttons::ROT_SPEED,
		sf::Color::White
	);
}

const char* Menu::get_option_text(Settings::Option op) const
{
	switch (op)
	{
	case Settings::Option::LEFT:
		return "Low";
	case Settings::Option::MIDDLE:
		return "Normal";
	case Settings::Option::RIGHT:
		return "High";
	}
	return "Error";
}

const char* Menu::get_color_text(Settings::Colors color) const
{
	switch (color)
	{
	case Settings::Colors::BLACK:
		return "Black";
	case Settings::Colors::RED:
		return "Red";
	case Settings::Colors::GREEN:
		return "Green";
	case Settings::Colors::BLUE:
		return "Blue";
	case Settings::Colors::CYAN:
		return "Cyan";
	case Settings::Colors::MAGENTA:
		return "Magenta";
	case Settings::Colors::YELLOW:
		return "Yellow";
	case Settings::Colors::WHITE:
		return "White";
	case Settings::Colors::GREY:
		return "Gray";
	case Settings::Colors::ORANGE:
		return "Orange";
	case Settings::Colors::BROWN:
		return "Brown";
	}
	return "Error";
}

const char* Menu::get_error_text(Parser::Error err) const
{
	switch (err)
	{
	case Parser::Error::SUCCESS:
		return "Ready to play!";
	case Parser::Error::NO_MAP:
		return "Please select map.";
	case Parser::Error::NO_TEXTURES:
		return "Please select all textures.";
	case Parser::Error::BAD_SYMBOL:
		return "Map should consist of \"01PC \" symbols.";
	case Parser::Error::BAD_BORDER:
		return "Map should be surrouded by walls.";
	case Parser::Error::NO_PATH:
		return "Player can't reach all the coins.";
	case Parser::Error::NO_PLAYER:
		return "No player in the map.";
	case Parser::Error::NO_COIN:
		return "No coin in the map.";
	case Parser::Error::MANY_PLAYERS:
		return "Map should contain 1 player.";
	}
	return "Error.";
}