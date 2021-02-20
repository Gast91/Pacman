#pragma once
#include <array>
#include "Config.h"

template <unsigned int animNo>
class Animation
{
protected:
    unsigned int currentAnim = 0;
    std::array<sf::IntRect, animNo> anims;
    std::unique_ptr<sf::Texture> spriteSheet;
public:
    virtual ~Animation() {}
    virtual inline unsigned int nextPos() { return ++currentAnim %= animNo; }
    virtual sf::IntRect& next(const sf::Vector2i direction = {0, 0}) { return anims[nextPos()]; }
    const sf::Texture& getTexture() const { return *spriteSheet; }
    void setTexture(const char* texture) { spriteSheet = Util::loadTexture(texture); }
};

template <unsigned int animNo, unsigned int perMove>
class MovementAnimation : public Animation<animNo>
{
private:
    unsigned int offset = 0;
public:
    MovementAnimation()
    {
        for (int i = 0, j = 0; i < animNo - 1; i += 2, ++j)
        {
            Animation<animNo>::anims[i]      = { 0                    ,  j * Config::sprites::size, Config::sprites::size, Config::sprites::size };
            Animation<animNo>::anims[i + 1]  = { Config::sprites::size,  j * Config::sprites::size, Config::sprites::size, Config::sprites::size };
        }
    }
    virtual ~MovementAnimation() {}
    virtual inline unsigned int nextPos() override { return (++Animation<animNo>::currentAnim %= perMove) + offset; }
    virtual sf::IntRect& next(const sf::Vector2i direction = {0, 0}) override
    {
        if (direction == sf::Vector2i{0, 0}) throw std::exception("Unspecified Animation Direction");
        offset = Config::offsetDict.at({direction.x, direction.y});
	    return Animation<animNo>::anims[nextPos()];
    }
};

template <unsigned int animNo>
struct HuntedAnimation : public Animation<animNo>
{
    HuntedAnimation()
    {
        for (int i = 0; i < animNo; ++i)
            Animation<animNo>::anims[i] = { i * Config::sprites::size, 0, Config::sprites::size, Config::sprites::size };
    }
    virtual ~HuntedAnimation() {}
};

template <unsigned int animNo>
struct EatenAnimation: public Animation<animNo>
{
    EatenAnimation()
    {
        for (int i = 0; i < animNo; ++i)
            Animation<animNo>::anims[i] = { i * Config::sprites::size, Config::sprites::size, Config::sprites::size, Config::sprites::size };
    }
    virtual ~EatenAnimation() {}
    virtual sf::IntRect& next(const sf::Vector2i direction = {0, 0}) override
    {
        if      (direction == NORTH) return  Animation<animNo>::anims[0];
        else if (direction == SOUTH) return  Animation<animNo>::anims[1];
        else if (direction == WEST)  return  Animation<animNo>::anims[2];
        else if (direction == EAST)  return  Animation<animNo>::anims[3];
        throw std::exception("Unspecified Animation Direction");
    }
};

template<unsigned int animNo>
class DeathAnimation : public Animation<animNo>
{
private:
    unsigned int progress = 0;
    bool done = false;
public:
    DeathAnimation()
    {
        for (int i = 0; i < animNo; ++i)
            Animation<animNo>::anims[i] = { i * Config::sprites::size, 0, Config::sprites::size, Config::sprites::size };
    }
    virtual ~DeathAnimation() {}
    virtual sf::IntRect& next(const sf::Vector2i direction = { 0, 0 }) override
    {
        if (++progress == animNo - 1) done = true;
        return Animation<animNo>::next();
    }
    void reset() { progress = 0; done = false; }
    bool isDone() const { return done; }
};