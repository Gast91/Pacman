#pragma once
#include "Level.h"
#include "Animation.h"

class Entity : public sf::Sprite
{
protected:
    sf::Vector2i initialGridPos;
    sf::Vector2i gridPosition;
    sf::Vector2i direction;
    float velocity = Config::ENTITY_SIZE;
    bool inBetween = false;

    MovementAnimation<8, 2> movAnim;

    virtual void updateAnimation(const sf::Vector2i& direction);
public:
    Entity(const char* spriteSheet, sf::Vector2i gridPos);
    virtual ~Entity() = default;

    virtual void move() = 0;
    virtual void changeDirection(const sf::Vector2i& target) = 0;
};