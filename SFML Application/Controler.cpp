#include "Controler.h"
#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "Parser.h"
#include "Settings.h"
#include "Page.h"
#include "Gameplay.h"

Controler::Controler() {}

void Controler::run()
{
	const std::size_t WINDOW_WIDTH = 1024; //1024
	const std::size_t WINDOW_HEIGHT = 768; //768
	Page current = Page::MENU_MAIN;
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Raycaster 3D", sf::Style::Close);
	sf::Event event;
	Settings settings;
	Parser parser(settings);
	Menu menu(window, settings, parser);
	Gameplay gameplay(window, settings, parser);
	bool game_started = false;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::EventType::Closed)
				window.close();
			switch (current)
			{
			case Page::MENU_MAIN:
				menu.handle_menu_events(event, current);
				break;
			case Page::MENU_SETTINGS:
				menu.handle_settings_events(event, current);
				break;
			case Page::GAMEPLAY:
				gameplay.handle_events(event, current, game_started);
				break;
			}
		}
		switch (current)
		{
		case Page::MENU_MAIN:
			menu.render_menu();
			break;
		case Page::MENU_SETTINGS:
			menu.render_settings();
			break;
		case Page::GAMEPLAY:
			if (!game_started)
			{
				game_started = true;
				gameplay.init();
			}
			gameplay.update();
			break;
		}
		window.display();
	}
}