#pragma once
#include "Ghost.h"

class Pinky : public Ghost
{
private:
    virtual void updateTarget(std::pair<sf::Vector2i, sf::Vector2i> pacMovement) override
    {
        if      (state == GhostState::Chase)
            target = Util::clampToGrid({ pacMovement.first.x + pacMovement.second.x * 4, pacMovement.first.y + pacMovement.second.y * 4 });
        else if (state == GhostState::Scatter) target = scatterTarget;
        else                                   target = frightenedTarget;
    }
public:
    Pinky(const AStar* astar, sf::Vector2i gridPos, sf::Vector2i scatterPos, sf::Vector2i frightenedPos)
        : Ghost(Config::sprites::pinky, astar, gridPos, scatterPos, frightenedPos) {}
    virtual ~Pinky() {}
};

class Inky : public Ghost
{
private:
    virtual void updateTarget(std::pair<sf::Vector2i, sf::Vector2i> pacMovement) override
    {
        if (state == GhostState::Chase)
        {
            const sf::Vector2i ahead = { pacMovement.first.x + pacMovement.second.x * 2, pacMovement.first.y + pacMovement.second.y * 2 };
            const sf::Vector2i blinkyVec = ahead - observers.front()->getCoords();
            target = Util::clampToGrid({ blinkyVec.x * 2, blinkyVec.y * 2 });
        }
        else if (state == GhostState::Scatter) target = scatterTarget;
        else                                   target = frightenedTarget;
    }
public:
    Inky(const AStar* astar, sf::Vector2i gridPos, sf::Vector2i scatterPos, sf::Vector2i frightenedPos)
        : Ghost(Config::sprites::inky, astar, gridPos, scatterPos, frightenedPos) 
    { 
        state = GhostState::Waiting;
        updateAnimation(EAST);
    }
    virtual ~Inky() {}

    virtual void updateState(GhostState gs) override
    {
        if ((state == GhostState::Waiting && aStar->canExit(30)) || state != GhostState::Waiting) state = gs;
    }
};

class Clyde : public Ghost
{
private:
    virtual void updateTarget(std::pair<sf::Vector2i, sf::Vector2i> pacMovement) override
    {
        if      (state == GhostState::Chase)
            target = Util::distance(Util::coordsToPosition(pacMovement.first), sprite.getPosition()) <= 8 * Config::ENTITY_SIZE ? scatterTarget : pacMovement.first;
        else if (state == GhostState::Scatter) target = scatterTarget;
        else                                   target = frightenedTarget;
    }
public:
    Clyde(const AStar* astar, sf::Vector2i gridPos, sf::Vector2i scatterPos, sf::Vector2i frightenedPos)
        : Ghost(Config::sprites::clyde, astar, gridPos, scatterPos, frightenedPos) 
    { 
        state = GhostState::Waiting; 
        updateAnimation(WEST);
    }
    virtual ~Clyde() {}

    virtual void updateState(GhostState gs) override
    {
        if ((state == GhostState::Waiting && aStar->canExit(80)) || state != GhostState::Waiting) state = gs;
    }
};