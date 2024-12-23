#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include "tinyfiledialogs.h"

class Settings
{
public:
	enum class Colors
	{
		BLACK,
		RED,
		GREEN,
		BLUE,
		CYAN,
		MAGENTA,
		YELLOW,
		WHITE,
		GREY,
		BROWN,
		ORANGE,
	};
	enum class Option {	LEFT, MIDDLE, RIGHT	};
	Settings();

	//Setters
	void set_default();
	void set_path();
	void incr_fov();
	void incr_sky_color();
	void incr_ground_color();
	void set_NO_texture();
	void set_SO_texture();
	void set_EA_texture();
	void set_WE_texture();
	void set_coin_texture();
	void incr_mov_speed();
	void incr_rot_speed();
	void switch_minimap();
	void switch_sound();

	//Getters
	std::string get_path() const;
	Option get_fov() const;
	Colors get_sky_color() const;
	Colors get_ground_color() const;
	std::string get_NO_texture() const;
	std::string get_SO_texture() const;
	std::string get_EA_texture() const;
	std::string get_WE_texture() const;
	std::string get_coin_texture() const;
	Option get_mov_speed() const;
	Option get_rot_speed() const;
	bool get_minimap() const;
	bool get_sound() const;
	sf::Color get_color(Colors color) const;
private:
	std::string path;
	Option		fov;
	Colors		sky_color;
	Colors		ground_color;
	std::string	NO_texture;
	std::string SO_texture;
	std::string	WE_texture;
	std::string EA_texture;
	std::string	coin_texture;
	Option		mov_speed;
	Option		rot_speed;
	bool		minimap;
	bool		sound;
};

#endif //SETTINGS_H_
