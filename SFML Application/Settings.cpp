#include "Settings.h"
#include <fstream>
#include <SFML/Graphics.hpp>

Settings::Settings()
{
	set_default();
}

void Settings::set_default() 
{
	path = "..\\Resources\\Maps\\subject_map.txt";
	NO_texture = "..\\Resources\\Game Textures\\brick_wall.png";
	SO_texture = "..\\Resources\\Game Textures\\blue_wall.png";
	EA_texture = "..\\Resources\\Game Textures\\steel_wall.png";
	WE_texture = "..\\Resources\\Game Textures\\stone_wall.png";
	coin_texture = "..\\Resources\\Game Textures\\coin.png";

	fov = Option::MIDDLE;
	sky_color = Colors::CYAN;
	ground_color = Colors::BROWN;
	mov_speed = Option::MIDDLE;
	rot_speed = Option::MIDDLE;
	minimap = true;
	sound = true;
}

#include <iostream>
void Settings::set_path()
{
	const char* format[] = { "*.txt" };
	const char* temp = tinyfd_openFileDialog(
		"Select a Map",          // Title
		"",                       // Default path
		1,                        // Number of file filters
		format,                  // File filters (e.g., "*.png")
		nullptr,                  // Description of filters
		0                        // Allow multiple selection (0 or 1)
	);
	if (temp)
		path = temp;
}

void Settings::incr_fov()
{
	fov = static_cast<Settings::Option>(((int)fov + 1) % 3);
}

void Settings::incr_sky_color()
{
	sky_color = static_cast<Settings::Colors>(((int)sky_color + 1) % 11);
}

void Settings::incr_ground_color()
{ 
	ground_color = static_cast<Settings::Colors>(((int)ground_color + 1) % 11);
}

void Settings::set_NO_texture() 
{ 
	const char* format[] = { "*.png", "*.jpg" };
	const char* temp = tinyfd_openFileDialog("Select a North Wall texture", "", 2, format, nullptr, 0);
	if (temp)
		NO_texture = temp;
}
void Settings::set_SO_texture() 
{ 
	const char* format[] = { "*.png", "*.jpg" };
	const char* temp = tinyfd_openFileDialog("Select a South Wall texture", "", 2, format, nullptr, 0);
	if (temp)
		SO_texture = temp;
}
void Settings::set_EA_texture()
{ 
	const char* format[] = { "*.png", "*.jpg" };
	const char* temp = tinyfd_openFileDialog("Select a East Wall texture", "", 2, format, nullptr, 0);
	if (temp)
		EA_texture = temp;
}
void Settings::set_WE_texture()
{
	const char* format[] = { "*.png", "*.jpg" };
	const char* temp = tinyfd_openFileDialog("Select a West Wall texture", "", 2, format, nullptr, 0);
	if (temp)
		WE_texture = temp;
}

void Settings::set_coin_texture()
{
	const char* format[] = { "*.png", "*.jpg" };
	const char* temp = tinyfd_openFileDialog("Select coin texture",	"",	2, format, nullptr, 0);
	if (temp)
		coin_texture = temp;
}

void Settings::incr_mov_speed()
{
	mov_speed = static_cast<Settings::Option>(((int)mov_speed + 1) % 3);
}

void Settings::incr_rot_speed() 
{
	rot_speed = static_cast<Settings::Option>(((int)rot_speed + 1) % 3);
}

void Settings::switch_minimap() { minimap = !minimap; }

void Settings::switch_sound() { sound = !sound; }

sf::Color Settings::get_color(Settings::Colors color) const
{
	switch (color)
	{
	case Settings::Colors::BLACK:
		return sf::Color::Black;
	case Settings::Colors::BLUE:
		return sf::Color::Blue;
	case Settings::Colors::CYAN:
		return sf::Color::Cyan;
	case Settings::Colors::GREEN:
		return sf::Color::Green;
	case Settings::Colors::MAGENTA:
		return sf::Color::Magenta;
	case Settings::Colors::RED:
		return sf::Color::Red;
	case Settings::Colors::WHITE:
		return sf::Color::White;
	case Settings::Colors::YELLOW:
		return sf::Color::Yellow;
	case Settings::Colors::GREY:
		return sf::Color(0x88, 0x88, 0x88);
	case Settings::Colors::BROWN:
		return sf::Color(0x89, 0x51, 0x29);
	case Settings::Colors::ORANGE:
		return sf::Color(0xFF, 0x5B, 0x00);
	}
	return sf::Color::Black;
}

std::string Settings::get_path() const { return path; }
Settings::Option Settings::get_fov() const { return fov; }
Settings::Colors Settings::get_sky_color() const { return sky_color; }
Settings::Colors Settings::get_ground_color() const { return ground_color; }
std::string Settings::get_NO_texture() const { return NO_texture; }
std::string Settings::get_SO_texture() const { return SO_texture; }
std::string Settings::get_EA_texture() const { return EA_texture; }
std::string Settings::get_WE_texture() const { return WE_texture; }
std::string Settings::get_coin_texture() const { return coin_texture; }
Settings::Option Settings::get_mov_speed() const { return mov_speed; }
Settings::Option Settings::get_rot_speed() const { return rot_speed; }
bool Settings::get_minimap() const { return minimap; }
bool Settings::get_sound() const { return sound; }