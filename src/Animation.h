#pragma once
#include <array>
#include "Config.h"

template <unsigned int animNo>
struct Animation
{
private:
    unsigned int currentAnim = 0;
public:
    virtual ~Animation() {}
    virtual inline unsigned int next() { return ++currentAnim %= animNo; }
    virtual sf::IntRect& next(const sf::Vector2i direction) = 0;
};

template <unsigned int animNo>
struct MovementAnimation : public Animation<animNo>
{
private:
    std::array<sf::IntRect, animNo> up;
    std::array<sf::IntRect, animNo> down;
    std::array<sf::IntRect, animNo> left;
    std::array<sf::IntRect, animNo> right;
public:
    MovementAnimation()
    {
        for (int i = 0; i < animNo; ++i)
        {
            right[i] = { i * Config::sprites::size,  0                        , Config::sprites::size, Config::sprites::size };
            left[i]  = { i * Config::sprites::size,      Config::sprites::size, Config::sprites::size, Config::sprites::size };
            up[i]    = { i * Config::sprites::size,  2 * Config::sprites::size, Config::sprites::size, Config::sprites::size };
            down[i]  = { i * Config::sprites::size,  3 * Config::sprites::size, Config::sprites::size, Config::sprites::size };
        }
    }
    virtual ~MovementAnimation() {}
    virtual sf::IntRect& next(const sf::Vector2i direction) override
    {
        if      (direction == NORTH) return    up.at(Animation<animNo>::next());
        else if (direction == SOUTH) return  down.at(Animation<animNo>::next());
        else if (direction == WEST)  return  left.at(Animation<animNo>::next());
        else if (direction == EAST)  return right.at(Animation<animNo>::next());
        throw std::exception("Unspecified Animation Direction");
    }
};

template <unsigned int animNo>
struct HuntedAnimation : public Animation<animNo>
{
private:
    std::array<sf::IntRect, animNo> frightened;
    std::array<sf::IntRect, animNo> deadMove;
public:
    HuntedAnimation()
    {
        for (int i = 0; i < animNo; ++i)
        {
            frightened[i] = { i * Config::sprites::size, 0                    , Config::sprites::size, Config::sprites::size };
            deadMove[i]   = { i * Config::sprites::size, Config::sprites::size, Config::sprites::size, Config::sprites::size };
        }
    }
    virtual ~HuntedAnimation() {}
    sf::IntRect& nextFright() { return frightened[Animation<animNo>::next()]; }
    virtual sf::IntRect& next(const sf::Vector2i direction) override
    {
        if      (direction == NORTH) return deadMove.at(0);
        else if (direction == SOUTH) return deadMove.at(1);
        else if (direction == WEST)  return deadMove.at(2);
        else if (direction == EAST)  return deadMove.at(3);
        throw std::exception("Unspecified Animation Direction");
    }
};

// make the arrays constants
// pacman's dead animation??