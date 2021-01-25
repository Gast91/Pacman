#pragma once
#include "Entity.h"

class Pacman : public Entity, public PacmanObserver  // transformable for getPosition?
{
private:
	sf::Vector2i nextTurn = EAST;
public:
	Pacman(const Level* lvl, sf::Vector2i gridPos);
	virtual ~Pacman();

    // PacmanObserver Interface Implementation
    virtual void teleport(int newX) override;
    virtual sf::Vector2i getGridPos() override;

    //Entity Interface Implementation
	virtual void changeDirection(const sf::Vector2i nxtTurn) override;
	virtual void move() override;
};