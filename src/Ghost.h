#pragma once
#include "Entity.h"
#include "AStar.h"

class Ghost : public Entity, public GhostObserver, public Subject
{
protected:
	const AStar* aStar;
    bool inBetween = false;

    sf::Vector2i target;
    const sf::Vector2i scatterTarget;
    const sf::Vector2i frightenedTarget;
    GhostState state = GhostState::Scatter;

	const std::unique_ptr<sf::Texture> huntedSpritesheet;  // SAME FOR ALL!!!!!! STATIC
    HuntedAnimation<4> huntedAnim;

    std::deque<std::unique_ptr<Node>> path;
    sf::VertexArray pathLines;

    virtual void updateAnimation(const sf::Vector2i direction) override;
public:
    Ghost(const char* spriteSheet, const AStar* astar, sf::Vector2i gridPos, sf::Vector2i scatterPos, sf::Vector2i frightenedPos);
    virtual ~Ghost() {};

	virtual void move() override;
    virtual void changeDirection(const sf::Vector2i target) override;

    virtual void notifyObservers(GhostState gs) override {}

    virtual void updateState(GhostState gs);
    virtual void updateTarget(std::pair<sf::Vector2i, sf::Vector2i> pacMovement) override;
    virtual GhostState getState() const override;
    virtual bool isNearHome() const override;
    virtual sf::Vector2i getCoords() const override;

    const sf::VertexArray& debugLines(const sf::Color color);
};