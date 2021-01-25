#pragma once
#include "Level.h"
#include "Config.h"

struct Animation
{
	sf::IntRect up[2];
	sf::IntRect down[2];
	sf::IntRect left[2];
	sf::IntRect right[2];
	unsigned int currentAnim = 0;

	unsigned int next() { return ++currentAnim %= 2; }
};

class Entity : public sf::Drawable
{
protected:
	const int SIZE = 16; // size in config already but as a float - only used in loadAnimations and also dependant on the sprite SIZE
    float velocity = Config::ENTITY_SIZE;

    sf::Vector2i direction;// = EAST;

	sf::Texture spriteSheet;
	sf::Sprite sprite;

	Animation movAnim;

	sf::Vector2i gridPosition;
	Level* level;

	virtual void loadAnimations();
    virtual void updateAnimation(const sf::Vector2i direction);
public:
	Entity(const char* spriteSheet, Level* lvl, sf::Vector2i gridPos);
	virtual ~Entity();

	virtual void move() = 0;
    virtual void changeDirection(const sf::Vector2i target) = 0;

	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};