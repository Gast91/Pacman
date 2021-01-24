#include "Tile.h"

Tile::Tile() {}
Tile::Tile(sf::Vector2i gridCoords, sf::Vector2f pos, int type, Node * p)
{
    node.gridPosition = gridCoords;
    node.parent = p;

    dotSprite.setFillColor(sf::Color::Yellow);
    dotSprite.setPosition(pos);

    switch (type)
    {
    case 0:
        dot = true;
        dotSprite.setRadius(Config::DOT_SIZE);
        break;
    case 1:
        wall = true;
        break;
    case 2:
        bigDot = true;
        dotSprite.setRadius(Config::B_DOT_SIZE);
        break;
    case 7:
        gate = true;
        break;
    case 9:
        teleporter = true;
        break;
    }
}

Tile::~Tile() {}

bool Tile::hasADot() const { return (dot || bigDot); }

void Tile::setEaten()
{
	dot    = false;
	bigDot = false;
}

void Tile::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (dot || bigDot)	target.draw(dotSprite);
}