#pragma once
#include "Entity.h"

class Pacman : public Entity, public PacmanObserver
{
private:
	const Level* level;
	sf::Vector2i nextTurn = EAST;

	//bool isDead = false;

	//const std::unique_ptr<sf::Texture> deathSpritesheet;
	DeathAnimation<12> deathAnim;
public:
	Pacman(const Level* lvl, sf::Vector2i gridPos);
	virtual ~Pacman() = default;

    // PacmanObserver Interface Implementation
    virtual void teleport(int newX) override;
    virtual std::pair<sf::Vector2i, sf::Vector2i> getMovement() const override;
	//notify dead?

    //Entity Interface Implementation
	virtual void changeDirection(const sf::Vector2i nxtTurn) override;
	virtual void move() override;
};