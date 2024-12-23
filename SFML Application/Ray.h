#ifndef RAY_H_
#define RAY_H_

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Ray
{
public:
    enum class Wall_Direction { NO, SO, EA, WE };

    struct Wall_Hit_Info
    {
        Wall_Direction direction;
        float wall_x;
        int side;
    };

    Ray(const sf::Vector2f& position, const sf::Vector2f& direction);

    float cast(const std::vector<std::string>& map);
    Wall_Hit_Info get_wall_hit_info() const;
    sf::Vector2i get_map_position() const;

private:
    sf::Vector2f pos;
    sf::Vector2f dir;
    sf::Vector2i map_pos;
    Wall_Hit_Info hit_info;
};

#endif // RAY_H_
