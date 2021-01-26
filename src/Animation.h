#pragma once
#include <array>
#include "Config.h"

// Individual Sprite Size
static const int SIZE = 16; 

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
private:
    std::array<sf::IntRect, size> up;
    std::array<sf::IntRect, size> down;
    std::array<sf::IntRect, size> left;
    std::array<sf::IntRect, size> right;
public:
    MovementAnimation()
    {
        for (int i = 0; i < size; ++i)
        {
            right[i] = { i * SIZE,  0,        SIZE, SIZE };
            left[i]  = { i * SIZE,  SIZE,     SIZE, SIZE };
            up[i]    = { i * SIZE,  2 * SIZE, SIZE, SIZE };
            down[i]  = { i * SIZE,  3 * SIZE, SIZE, SIZE };
        }
    }
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
private:
    std::array<sf::IntRect, size> frightened;
    std::array<sf::IntRect, size> deadMove;
public:
    HuntedAnimation()
    {
        for (int i = 0; i < size; ++i)
        {
            frightened[i] = { i * SIZE, 0   , SIZE, SIZE };
            deadMove[i]   = { i * SIZE, SIZE, SIZE, SIZE };
        }
    }
    sf::IntRect& nextFright() { return frightened[Animation<size>::next()]; }
    virtual sf::IntRect& next(const sf::Vector2i direction) override
    {
        if      (direction == NORTH) return deadMove.at(0);
        else if (direction == SOUTH) return deadMove.at(1);
        else if (direction == WEST)  return deadMove.at(2);
        else if (direction == EAST)  return deadMove.at(3);
    }
};

// make the arrays constants
// pacman's dead animation??