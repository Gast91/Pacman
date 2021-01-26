#pragma once
#include "Entity.h"
#include "AStar.h"

//#define CLASSIC

class Ghost : public Entity, public GhostObserver  // transformable for getPosition?
{
protected:
	AStar* aStar;    // not like this - remove - factory based on personality
    bool inBetween = false;

    sf::Vector2i target;
    sf::Vector2i scatterTarget;
    sf::Vector2i frightenedTarget;   // CONSISTENT NAMES
    GhostState state = Scatter;

	sf::Texture huntedSpritesheet;  // SAME FOR ALL!!!!!! STATIC
    HuntedAnimation<4> huntedAnim;

    virtual void updateAnimation(const sf::Vector2i direction) override;
public:
	Ghost(const char* spriteSheet, const char* huntedSpritesheet, const Level* lvl, sf::Vector2i gridPos, sf::Vector2i scatterPos, sf::Vector2i frightenedPos);
	virtual ~Ghost();

	virtual void move() override;
    virtual void changeDirection(const sf::Vector2i target) override;

    virtual void updateState(GhostState gs) override;
    virtual GhostState getState() override;
    virtual bool isNearHome() override;
    virtual sf::Vector2f getPos() override;

    std::deque<std::unique_ptr<Node>> path;
};

// some ghosts stay in the house until a number of dots have been eaten
// use classic mode for pinky and inky if target is outside the grid???

// pinky's goal is 4 tiles ahead of pacman's location and direction
// inky's goal is calculated as such: find position 2 tiles ahead of pacman's location and direction,
// draw vector from blinky's location to that tile and double it. Resulting position is inky's target
// clyde's goal is the same as blinky's as long as its distance from pacman is more than 8 tiles (manhattan or euclidean?)
// if less than 8 tiles, its target is the bottom left corner (outside the maze...).