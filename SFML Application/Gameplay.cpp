#include "Gameplay.h"
#include "Minimap.h"
#include "Settings.h"
#include "Parser.h"
#include "Ray.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

Gameplay::Gameplay(sf::RenderWindow& window_, Settings& settings_, Parser& parser_) :
	window(window_), settings(settings_), parser(parser_), coins(0), 
	minimap(window), score(0), max_score(0)
{
	if (!font.loadFromFile("..\\Resources\\Fonts\\Silkscreen.ttf"))
		font.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf");
	coin_sound_buffer.loadFromFile("..\\Resources\\Sounds\\coin_pickup.mp3");
	coin_sound.setBuffer(coin_sound_buffer);
	win_sound_buffer.loadFromFile("..\\Resources\\Sounds\\win.mp3");
	win_sound.setBuffer(win_sound_buffer);
}

void Gameplay::init()
{
	map = parser.get_map();
	coins = parser.get_coins();
	for (sf::Vector2f& v : coins)
	{
		v.x += 0.5f;
		v.y += 0.5f;
	}
	player.pos = static_cast<sf::Vector2f>(parser.get_player());
	player.pos.x += 0.5f;
	player.pos.y += 0.5f;
	player.up_enabled = false;
	player.down_enabled = false;
	player.left_enabled = false;
	player.right_enabled = false;
	player.sprint = false;
	map[player.pos.y][player.pos.x] = '.';
	max_score = coins.size();
	player.dir = sf::Vector2f(0, -1); //North
	switch (settings.get_fov())
	{
	case Settings::Option::LEFT:
		player.plane = sf::Vector2f(0.2679f, 0); //30deg
		break;
	case Settings::Option::MIDDLE:
		player.plane = sf::Vector2f(0.66f, 0); //60deg
		break;
	case Settings::Option::RIGHT:
		player.plane = sf::Vector2f(1.732f, 0); //120deg
		break;
	}
	switch (settings.get_mov_speed())
	{
	case Settings::Option::LEFT:
		player.base_mov_speed = 0.0025f;
		break;
	case Settings::Option::MIDDLE:
		player.base_mov_speed = 0.005f;
		break;
	case Settings::Option::RIGHT:
		player.base_mov_speed = 0.01f;
		break;
	}
	switch (settings.get_rot_speed())
	{
	case Settings::Option::LEFT:
		player.rot_speed = 0.01f;
		break;
	case Settings::Option::MIDDLE:
		player.rot_speed = 0.02f;
		break;
	case Settings::Option::RIGHT:
		player.rot_speed = 0.04f;
		break;
	}
	NO_texture.loadFromFile(settings.get_NO_texture());
	SO_texture.loadFromFile(settings.get_SO_texture());
	EA_texture.loadFromFile(settings.get_EA_texture());
	WE_texture.loadFromFile(settings.get_WE_texture());
	coin_texture.loadFromFile(settings.get_coin_texture());
	clk.restart();
}

void Gameplay::render_ground()
{
	sf::RectangleShape ground(sf::Vector2f(window.getSize().x, window.getSize().y / 2));
	ground.setPosition(sf::Vector2f(0, window.getSize().y / 2));
	ground.setFillColor(settings.get_color(settings.get_ground_color()));
	window.draw(ground);
}

void Gameplay::render()
{
    window.clear(settings.get_color(settings.get_sky_color()));
    render_ground();

    std::vector<float> z_buffer(window.getSize().x, std::numeric_limits<float>::infinity());

    for (int x = 0; x < static_cast<int>(window.getSize().x); ++x)
    {
        double camera_x = 2.0 * x / static_cast<double>(window.getSize().x) - 1;
        sf::Vector2f ray_dir = player.dir + player.plane * static_cast<float>(camera_x);

        Ray ray(player.pos, ray_dir);

        float dist_to_wall = ray.cast(map);
        sf::Vector2i map_cell = ray.get_map_position();
        Ray::Wall_Hit_Info hit_info = ray.get_wall_hit_info();

        int line_height = static_cast<int>(window.getSize().y / dist_to_wall);

        int draw_start = -line_height / 2 + window.getSize().y / 2;
        draw_start = std::max(draw_start, 0);

        int draw_end = line_height / 2 + window.getSize().y / 2;
        draw_end = std::min(draw_end, static_cast<int>(window.getSize().y) - 1);

        sf::Texture* wall_texture = nullptr;
        switch (hit_info.direction)
        {
        case Ray::Wall_Direction::NO:
            wall_texture = &NO_texture;
            break;
        case Ray::Wall_Direction::SO:
            wall_texture = &SO_texture;
            break;
        case Ray::Wall_Direction::EA:
            wall_texture = &EA_texture;
            break;
        case Ray::Wall_Direction::WE:
            wall_texture = &WE_texture;
            break;
        }

        float wall_x = hit_info.wall_x;
        int tex_x = static_cast<int>(wall_x * wall_texture->getSize().x);
        if (hit_info.side == 1)
            tex_x = wall_texture->getSize().x - tex_x - 1;

		sf::VertexArray wall(sf::Quads, 4);

		float texture_ratio = static_cast<float>(wall_texture->getSize().y) / line_height;

		float tex_y = (draw_start - window.getSize().y / 2.f + line_height / 2.f) * texture_ratio;
		wall[0].position = sf::Vector2f(x, draw_start);
		wall[0].texCoords = sf::Vector2f(tex_x, tex_y);

		wall[1].position = sf::Vector2f(x + 1, draw_start);
		wall[1].texCoords = sf::Vector2f(tex_x + 1, tex_y);

		tex_y = (draw_end - window.getSize().y / 2.f + line_height / 2.f) * texture_ratio;
		wall[2].position = sf::Vector2f(x + 1, draw_end);
		wall[2].texCoords = sf::Vector2f(tex_x + 1, tex_y);

		wall[3].position = sf::Vector2f(x, draw_end);
		wall[3].texCoords = sf::Vector2f(tex_x, tex_y);

		window.draw(wall, wall_texture);

        // sf::Sprite wall_sprite(*wall_texture);
		// wall_sprite.setTextureRect(sf::IntRect(tex_x, tex_y, 1, wall_texture->getSize().y));
		// wall_sprite.setScale(1, static_cast<float>(line_height) / wall_texture->getSize().y);
		// wall_sprite.setPosition(x, draw_start);

		// window.draw(wall_sprite);

        for (int y = draw_start; y < draw_end; ++y)
        {
            float depth = dist_to_wall;
            if (depth < z_buffer[x])
                z_buffer[x] = depth;
        }
    }

	coins.erase(std::remove_if(coins.begin(), coins.end(),
		[this](const sf::Vector2f& coin_pos)
		{
			sf::Vector2f diff = coin_pos - player.pos;
			float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

			if (distance < 0.5f)
			{
				map[coin_pos.y][coin_pos.x] = '.';
				++score;
				if (settings.get_sound())
				{
					if (score == max_score)
						win_sound.play();
					coin_sound.play();
				}
			}
			return distance < 0.5f;
		}), coins.end());

    std::sort(coins.begin(), coins.end(), [this](const sf::Vector2f& a, const sf::Vector2f& b)
        {
            float dist_a = calculate_coin_distance(a).y;
            float dist_b = calculate_coin_distance(b).y;
            return dist_a > dist_b;
        });

    for (const sf::Vector2f& coin_pos : coins)
        render_coin(coin_pos, z_buffer);
	if (settings.get_minimap())
		minimap.render(map, player);
	render_score();
}

sf::Vector2f Gameplay::calculate_coin_distance(const sf::Vector2f& coin)
{
    sf::Vector2f coin_pos = coin - player.pos;
	sf::Vector2f transform;

    float inv_det = 1.0f / (player.plane.x * player.dir.y - player.dir.x * player.plane.y);
    transform.x = inv_det * (player.dir.y * coin_pos.x - player.dir.x * coin_pos.y);
    transform.y = inv_det * (-player.plane.y * coin_pos.x + player.plane.x * coin_pos.y);

    return transform;
}

void Gameplay::render_coin(const sf::Vector2f& coin, const std::vector<float>& z_buffer)
{
	sf::Vector2f transform = calculate_coin_distance(coin);

	if (transform.y <= 0)
		return;

	int coin_screen_x = static_cast<int>((window.getSize().x / 2) * (1 + transform.x / transform.y));

	const float scaling_factor = 0.5f;
	int coin_height = static_cast<int>(std::abs(window.getSize().y / transform.y) * scaling_factor);
	int coin_width = coin_height;

	int draw_start_x = std::max(coin_screen_x - coin_width / 2.f, 0.f);
	int draw_end_x = std::min(coin_screen_x + coin_width / 2.f, static_cast<float>(window.getSize().x) - 1);
	int draw_start_y = std::max(window.getSize().y / 2.f - coin_height / 2.f, 0.f);
	int draw_end_y = std::min(window.getSize().y / 2.f + coin_height / 2.f, static_cast<float>(window.getSize().y) - 1);

	for (int x = draw_start_x; x < draw_end_x; ++x)
	{
		if (transform.y < z_buffer[x])
		{
			int tex_x = static_cast<int>((x - (coin_screen_x - coin_width / 2)) * coin_texture.getSize().x / coin_width);

			sf::Sprite coin(coin_texture);
			coin.setTextureRect(sf::IntRect(tex_x, 0, 1, coin_texture.getSize().y));
			coin.setScale(1, static_cast<float>(coin_height) / coin_texture.getSize().y);
			coin.setPosition(static_cast<float>(x), static_cast<float>(draw_start_y));

			window.draw(coin);
		}
	}
}


void Gameplay::render_score()
{
	const sf::Vector2f	WINDOW_SIZE = static_cast<sf::Vector2f>(window.getSize());
	const std::size_t	OUTLINE_THICKNESS = 8;

	sf::Text collected(
		"Collected: " + std::to_string(score) + " / " + std::to_string(max_score),
		font,
		36
	);
	sf::FloatRect score_bounds = collected.getLocalBounds();
	collected.setOrigin(score_bounds.width / 2.f, score_bounds.height / 2.f);
	collected.setPosition(WINDOW_SIZE.x * 0.75f, WINDOW_SIZE.y * 0.1f);
	collected.setOutlineThickness(OUTLINE_THICKNESS);
	window.draw(collected);

	std::size_t seconds = static_cast<std::size_t>(clk.getElapsedTime().asSeconds()) % 60;
	std::size_t minutes = static_cast<std::size_t>(clk.getElapsedTime().asSeconds()) / 60;
	sf::Text time(
		"Time: " + std::to_string(minutes) + " : " + std::to_string(seconds),
		font,
		30
	);
	sf::FloatRect time_bounds = time.getLocalBounds();
	time.setOrigin(time_bounds.width / 2.f, time_bounds.height / 2.f);
	time.setPosition(WINDOW_SIZE.x * 0.7f, WINDOW_SIZE.y * 0.16f);
	time.setOutlineThickness(OUTLINE_THICKNESS);
	window.draw(time);

	if (score == max_score)
	{
		sf::Text cong(
			"Congratulations!, You Won!",
			font,
			48
		);
		sf::FloatRect text_bounds = cong.getLocalBounds();
		cong.setOrigin(text_bounds.width / 2.f, text_bounds.height / 2.f);
		cong.setPosition(WINDOW_SIZE.x * 0.5f, WINDOW_SIZE.y * 0.22f);
		cong.setOutlineThickness(OUTLINE_THICKNESS);
		cong.setFillColor(sf::Color::Green);
		window.draw(cong);
	}
}

void Gameplay::handle_events(sf::Event& event, Page& current, bool& game_started)
{
	bool is_key_pressed = event.type == sf::Event::KeyPressed;

	if (is_key_pressed || event.type == sf::Event::KeyReleased) {
		if (event.key.code == sf::Keyboard::W)
			player.up_enabled = is_key_pressed;
		else if (event.key.code == sf::Keyboard::S)
			player.down_enabled = is_key_pressed;
		else if (event.key.code == sf::Keyboard::A)
			player.left_enabled = is_key_pressed;
		else if (event.key.code == sf::Keyboard::D)
			player.right_enabled = is_key_pressed;
		else if (event.key.code == sf::Keyboard::LShift)
			player.sprint = is_key_pressed;
		else if (event.key.code == sf::Keyboard::Escape)
		{
			clear();
			game_started = false;
			current = Page::MENU_MAIN;
		}
	}
	if (event.type == sf::Event::MouseMoved)
		rotate_mouse();
}

void Gameplay::move_forward()
{
	float speed = (player.sprint) ? 2 * player.mov_speed : player.mov_speed;
	sf::Vector2f new_pos = player.pos + player.dir * speed;

	if (new_pos.y >= 0 && new_pos.y < map.size() &&
		map[new_pos.y][player.pos.x] != '1')
		player.pos.y = new_pos.y;
	if (new_pos.x >= 0 && new_pos.x < map[static_cast<int>(player.pos.y)].size() &&
		map[player.pos.y][new_pos.x] != '1')
		player.pos.x = new_pos.x;
}
void Gameplay::move_backward()
{
	float speed = (player.sprint) ? 2 * player.mov_speed : player.mov_speed;
	sf::Vector2f new_pos = player.pos - player.dir * speed;

	if (new_pos.y >= 0 && new_pos.y < map.size() &&
		map[new_pos.y][player.pos.x] != '1')
		player.pos.y = new_pos.y;
	if (new_pos.x >= 0 && new_pos.x < map[static_cast<int>(player.pos.y)].size() &&
		map[player.pos.y][new_pos.x] != '1')
		player.pos.x = new_pos.x;
}
void Gameplay::move_right()
{
	float speed = (player.sprint) ? 2 * player.mov_speed : player.mov_speed;
	sf::Vector2f left_dir = sf::Vector2f(-player.dir.y, player.dir.x);

	sf::Vector2f new_pos = sf::Vector2f(
		player.pos.x + left_dir.x * speed,
		player.pos.y + left_dir.y * speed
	);

	if (new_pos.y >= 0 && new_pos.y < map.size() &&
		map[new_pos.y][player.pos.x] != '1')
		player.pos.y = new_pos.y;
	if (new_pos.x >= 0 && new_pos.x < map[static_cast<int>(player.pos.y)].size() &&
		map[player.pos.y][new_pos.x] != '1')
		player.pos.x = new_pos.x;
}


void Gameplay::move_left()
{
	float speed = (player.sprint) ? 2 * player.mov_speed : player.mov_speed;
	sf::Vector2f left_dir = sf::Vector2f(-player.dir.y, player.dir.x);

	sf::Vector2f new_pos = sf::Vector2f(
		player.pos.x - left_dir.x * speed,
		player.pos.y - left_dir.y * speed
	);

	if (new_pos.y >= 0 && new_pos.y < map.size() &&
		map[new_pos.y][player.pos.x] != '1')
		player.pos.y = new_pos.y;
	if (new_pos.x >= 0 && new_pos.x < map[static_cast<int>(player.pos.y)].size() &&
		map[player.pos.y][new_pos.x] != '1')
		player.pos.x = new_pos.x;
}

void Gameplay::rotate_mouse()
{
	sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
	sf::Vector2i center(window.getSize().x / 2, window.getSize().y / 2);

	if (mouse_pos != center)
	{
		float delta_x = mouse_pos.x - center.x;

		float rot_angle = delta_x * player.rot_speed / 10.f;

		float old_dir_x = player.dir.x;
		player.dir.x = player.dir.x * cos(rot_angle) - player.dir.y * sin(rot_angle);
		player.dir.y = old_dir_x * sin(rot_angle) + player.dir.y * cos(rot_angle);

		float old_plane_x = player.plane.x;
		player.plane.x = player.plane.x * cos(rot_angle) - player.plane.y * sin(rot_angle);
		player.plane.y = old_plane_x * sin(rot_angle) + player.plane.y * cos(rot_angle);

		sf::Mouse::setPosition(center, window);
	}
}

void Gameplay::clear()
{
	map.clear();
	coins.clear();
	score = 0;
	max_score = 0;
}

void Gameplay::update()
{
	player.mov_speed = player.base_mov_speed * dt.restart().asMilliseconds();
	if (player.up_enabled)
		move_forward();
	if (player.down_enabled)
		move_backward();
	if (player.left_enabled)
		move_left();
	if (player.right_enabled)
		move_right();
	render();
}