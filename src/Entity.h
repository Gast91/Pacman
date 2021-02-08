#pragma once
#include "Level.h"
#include "Animation.h"

class Entity : public sf::Drawable//, public sf::Transformable    // does transformable work?
{
protected:
	sf::Vector2i gridPosition;
    sf::Vector2i direction;
    float velocity = Config::ENTITY_SIZE;

	//const std::unique_ptr<sf::Texture> spriteSheet;
	sf::Sprite sprite;

    MovementAnimation<8, 2> movAnim;

    virtual void updateAnimation(const sf::Vector2i direction);
public:
	Entity(const char* spriteSheet, sf::Vector2i gridPos);
	virtual ~Entity() = default;

	virtual void move() = 0;
    virtual void changeDirection(const sf::Vector2i target) = 0;

	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};