#include "Tile.h"

Tile::Tile(sf::Vector2i gridCoords, sf::Vector2f pos, int type, Node* p) : type(static_cast<TileType>(type)), node{ gridCoords, p }, dotSprite([&]
{
    std::unique_ptr<sf::CircleShape> sprite;
    if (Tile::type == TileType::Dot || Tile::type == TileType::BigDot)
    {
        sprite = std::make_unique<sf::CircleShape>(Tile::type == TileType::Dot ? Config::DOT_SIZE : Config::B_DOT_SIZE);
        sprite->setFillColor(sf::Color::Yellow);
        sprite->setPosition(pos);
    }
    return std::move(sprite);
}()) {}

bool Tile::hasADot() const { return (type == TileType::Dot || type == TileType::BigDot) && !eaten; }

int  Tile::getValue() const { return type == TileType::Dot ? Config::DOT_VALUE : Config::BDOT_VALUE; }

void Tile::setEaten() { eaten = true; }

void Tile::reset() { if (type == TileType::Dot || type == TileType::BigDot) eaten = false; }

void Tile::draw(sf::RenderTarget &target, sf::RenderStates states) const { if (hasADot()) target.draw(*dotSprite); }