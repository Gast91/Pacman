#pragma once
#include <array>
#include "Config.h"

template <unsigned int size>
struct Animation
{
protected:
    unsigned int currentAnim = 0;
public:
    virtual inline unsigned int next() { return ++currentAnim %= size; }
    virtual sf::IntRect& next(const sf::Vector2i direction) = 0;
};

template <unsigned int size>
struct MovementAnimation : public Animation<size>
{
    std::array<sf::IntRect, size> up;
    std::array<sf::IntRect, size> down;
    std::array<sf::IntRect, size> left;
    std::array<sf::IntRect, size> right;

    virtual sf::IntRect& next(const sf::Vector2i direction) override
    {
        if      (direction == NORTH) return    up.at(Animation<size>::next());
        else if (direction == SOUTH) return  down.at(Animation<size>::next());
        else if (direction == WEST)  return  left.at(Animation<size>::next());
        else if (direction == EAST)  return right.at(Animation<size>::next());
    }
};

template <unsigned int size>
struct HuntedAnimation : public Animation<size>
{
    std::array<sf::IntRect, size> frightened;
    std::array<sf::IntRect, size> deadMove;

    virtual inline unsigned int next() override { return Animation<size>::next(); }
    virtual sf::IntRect& next(const sf::Vector2i direction) override
    {
        if      (direction == NORTH) return deadMove.at(0);
        else if (direction == SOUTH) return deadMove.at(1);
        else if (direction == WEST)  return deadMove.at(2);
        else if (direction == EAST)  return deadMove.at(3);
    }
};