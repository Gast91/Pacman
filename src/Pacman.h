#pragma once
#include "Entity.h"

class Pacman : public Entity, public PacmanObserver
{
private:
	const Level* level;
	sf::Vector2i nextTurn = EAST;

	DeathAnimation<13> deathAnim;
public:
	Pacman(const Level* lvl, sf::Vector2i gridPos);
	virtual ~Pacman() = default;

	void rip();

    // PacmanObserver Interface Implementation
    virtual void teleport(int newX) override;
    virtual std::pair<sf::Vector2i, sf::Vector2i> getMovement() const override;
	virtual sf::FloatRect getGlobalBounds() const override;

    //Entity Interface Implementation
	virtual void changeDirection(const sf::Vector2i nxtTurn) override;
	virtual void move() override;
};