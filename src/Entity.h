#pragma once
#include "Level.h"
#include "Animation.h"

class Entity : public sf::Drawable    // transformable for getPosition?
{
protected:
	const int SIZE = 16; // size in config already but as a float - only used in loadAnimations and also dependant on the sprite SIZE
    float velocity = Config::ENTITY_SIZE;

    sf::Vector2i direction;

	sf::Texture spriteSheet;
	sf::Sprite sprite;

    MovementAnimation<2> movAnim;

	sf::Vector2i gridPosition;
	const Level* level;

	virtual void loadAnimations();
    virtual void updateAnimation(const sf::Vector2i direction);
public:
	Entity(const char* spriteSheet, const Level* lvl, sf::Vector2i gridPos);
	virtual ~Entity();

	virtual void move() = 0;
    virtual void changeDirection(const sf::Vector2i target) = 0;

	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};