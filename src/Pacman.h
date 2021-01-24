#pragma once
#include "Entity.h"

class Pacman : public Entity
{
private:
	sf::Vector2i nextTurn = EAST;
public:
	Pacman(Level* lvl, sf::Vector2i gridPos);
	virtual ~Pacman();

	void changeDirection(const sf::Vector2i nxtTurn);
	virtual void move() override;
};