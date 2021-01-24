#pragma once
#include "Entity.h"
#include "AStar.h"

//#define CLASSIC

struct HuntedAnimation
{
    sf::IntRect frightened[4];
    sf::IntRect deadUp;
    sf::IntRect deadLeft;
    sf::IntRect deadRight;
    sf::IntRect deadDown;
    unsigned int currentAnim = 0;

    unsigned int next() { return ++currentAnim %= 4; }
};

//enum Personalities { Blinky, Inky, Pinky, Clyde };  // <---
enum GhostState    { Chase, Scatter, Frightened, Dead };  // <--- Scatter they go at their corners, freightened they go in the middle

class Ghost : public Entity
{
protected:
	AStar* aStar;    // not like this - remove - factory based on personality
    bool inBetween = false;

    sf::Vector2i scatterTarget;
    sf::Vector2i frightenedTarget;
    GhostState state = Scatter;

	sf::Texture huntedSpritesheet;
    HuntedAnimation huntedAnim;

	virtual void loadAnimations() override;
    virtual void updateAnimation(const sf::Vector2i direction) override;
public:
	Ghost(const char* spriteSheet, const char* huntedSpritesheet, Level* lvl, sf::Vector2i gridPos, sf::Vector2i scatterPos, sf::Vector2i frightenedPos);
	virtual ~Ghost();

	virtual void move() override;
    virtual void changeDirection(const sf::Vector2i target) override;
    void debugTargeting();

    std::deque<std::unique_ptr<Node>> path;
};

// Ghost's are not able to change direction..seems vital - might also solve problem - thus turn decisions are made on intersections only
// will be needed for scatter mode. Also for scatter mode their goal is outside the bounds of the maze...
// some ghosts stay in the house until a number of dots have been eaten

// THERE IS NO TURNING CALC FOR SOME INTERSECTIONS - need to keep track of the next intersection?

// pinky's goal is 4 tiles ahead of pacman's location and direction
// inky's goal is calculated as such: find position 2 tiles ahead of pacman's location and direction,
// draw vector from blinky's location to that tile and double it. Resulting position is inky's target
// clyde's goal is the same as blinky's as long as its distance from pacman is more than 8 tiles (manhattan or euclidean?)
// if less than 8 tiles, its target is the bottom left corner (outside the maze...).

// ghost states: Chase, Scatter, Frightened
//1. Scatter for 7 seconds, then Chase for 20 seconds.
//2. Scatter for 7 seconds, then Chase for 20 seconds.
//3. Scatter for 5 seconds, then Chase for 20 seconds.
//4. Scatter for 5 seconds, then switch to Chase mode permanently.
// Timer is paused when pacman eats big dot (ghost's go into frightened state)
// Timer is reset on new level (not relevant atm) and when pacman loses a life (dies)
