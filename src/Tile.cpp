#include "Tile.h"

Tile::Tile(sf::Vector2i gridCoords, sf::Vector2f pos, int type, Node* p) : type(static_cast<TileType>(type)), node{ gridCoords, p }
{
    if (hasADot())
    {
        dotSprite.setFillColor(sf::Color::Yellow);
        dotSprite.setPosition(pos);
        dotSprite.setRadius(Tile::type == TileType::Dot ? Config::DOT_SIZE : Config::B_DOT_SIZE);
    }
}

bool Tile::hasADot() const { return (type == TileType::Dot || type == TileType::BigDot) && !eaten; }

void Tile::setEaten() { eaten = true; }

void Tile::draw(sf::RenderTarget &target, sf::RenderStates states) const { if (hasADot()) target.draw(dotSprite); }