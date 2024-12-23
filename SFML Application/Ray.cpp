#include "Ray.h"
#include <cmath>
#include <limits>

Ray::Ray(const sf::Vector2f& position, const sf::Vector2f& direction)
    : pos(position), dir(direction) {}

float Ray::cast(const std::vector<std::string>& map)
{
    map_pos = sf::Vector2i(static_cast<int>(pos.x), static_cast<int>(pos.y));

    sf::Vector2f delta_dist(std::abs(1.0f / dir.x), std::abs(1.0f / dir.y));
    sf::Vector2f side_dist;
    sf::Vector2i step;

    if (dir.x < 0)
    {
        step.x = -1;
        side_dist.x = (pos.x - map_pos.x) * delta_dist.x;
    }
    else
    {
        step.x = 1;
        side_dist.x = (map_pos.x + 1.0f - pos.x) * delta_dist.x;
    }
    if (dir.y < 0)
    {
        step.y = -1;
        side_dist.y = (pos.y - map_pos.y) * delta_dist.y;
    }
    else
    {
        step.y = 1;
        side_dist.y = (map_pos.y + 1.0f - pos.y) * delta_dist.y;
    }

    bool hit = false;
    int side;
    while (!hit)
    {
        if (side_dist.x < side_dist.y)
        {
            side_dist.x += delta_dist.x;
            map_pos.x += step.x;
            side = 0;
        }
        else
        {
            side_dist.y += delta_dist.y;
            map_pos.y += step.y;
            side = 1;
        }

        if (map[map_pos.y][map_pos.x] == '1')
            hit = true;
    }

    float perp_wall_dist;
    if (side == 0)
        perp_wall_dist = (map_pos.x - pos.x + (1 - step.x) / 2) / dir.x;
    else
        perp_wall_dist = (map_pos.y - pos.y + (1 - step.y) / 2) / dir.y;

    hit_info.side = side;
    if (side == 0)
        hit_info.wall_x = pos.y + perp_wall_dist * dir.y;
    else
        hit_info.wall_x = pos.x + perp_wall_dist * dir.x;
    hit_info.wall_x -= std::floor(hit_info.wall_x);

    if (side == 0)
    {
        if (step.x > 0)
            hit_info.direction = Wall_Direction::EA;
        else
            hit_info.direction = Wall_Direction::WE;
    }
    else
    {
        if (step.y > 0)
            hit_info.direction = Wall_Direction::SO;
        else
            hit_info.direction = Wall_Direction::NO;
    }
    return perp_wall_dist;
}

Ray::Wall_Hit_Info Ray::get_wall_hit_info() const { return hit_info; }
sf::Vector2i Ray::get_map_position() const { return map_pos; }
