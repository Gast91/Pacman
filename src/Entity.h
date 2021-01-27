#pragma once
#include "Level.h"
#include "Animation.h"

class Entity : public sf::Drawable//, public sf::Transformable    // does transformable work?
{
protected:
    float velocity = Config::ENTITY_SIZE;

    sf::Vector2i direction;

	sf::Texture spriteSheet;
	sf::Sprite sprite;

    MovementAnimation<2> movAnim;

	sf::Vector2i gridPosition;
	const Level* level;

    virtual void updateAnimation(const sf::Vector2i direction);
public:
	Entity(const char* spriteSheet, const Level* lvl, sf::Vector2i gridPos);
	virtual ~Entity();

	virtual void move() = 0;
    virtual void changeDirection(const sf::Vector2i target) = 0;

	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};