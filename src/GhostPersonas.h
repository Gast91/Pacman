#pragma once
#include "Ghost.h"

class Pinky : public Ghost
{
private:
    virtual void updateTarget(std::pair<sf::Vector2i, sf::Vector2i> pacMovement) override
    {
        if (state == GhostState::Chase)
            target = { pacMovement.first.x + pacMovement.second.x * 4, pacMovement.first.y + pacMovement.second.y * 4 };
        else if (state == GhostState::Scatter) target = scatterTarget;
        else                                   target = frightenedTarget;
    }
public:
    Pinky(const Level* lvl, sf::Vector2i gridPos, sf::Vector2i scatterPos, sf::Vector2i frightenedPos)
        : Ghost(Config::sprites::pinky, lvl, gridPos, scatterPos, frightenedPos) {}
    virtual ~Pinky() {}
};

class Inky : public Ghost    // TUNNEL SHENANIGANS (CANNOT REVERSE AND ITS PATHFINDING LEADS IT THERE EVENTUALLY)
{
private:
    virtual void updateTarget(std::pair<sf::Vector2i, sf::Vector2i> pacMovement) override
    {
        if (state == GhostState::Chase)
        {
            const sf::Vector2i ahead = { pacMovement.first.x + pacMovement.second.x * 2, pacMovement.first.y + pacMovement.second.y * 2 };
            const sf::Vector2i blinkyVec = ahead - observers.front()->getCoords();
            target = { blinkyVec.x * 2, blinkyVec.y * 2 };
        }
        else if (state == GhostState::Scatter) target = scatterTarget;
        else                                   target = frightenedTarget;
    }
public:
    Inky(const Level* lvl, sf::Vector2i gridPos, sf::Vector2i scatterPos, sf::Vector2i frightenedPos)
        : Ghost(Config::sprites::inky, lvl, gridPos, scatterPos, frightenedPos) {}
    virtual ~Inky() {}
};

class Clyde : public Ghost
{
private:
    virtual void updateTarget(std::pair<sf::Vector2i, sf::Vector2i> pacMovement) override
    {
        if (state == GhostState::Chase)
            target = distance(coordsToPosition(pacMovement.first), sprite.getPosition()) <= 8 * Config::ENTITY_SIZE ? scatterTarget : pacMovement.first;
        else if (state == GhostState::Scatter) target = scatterTarget;
        else                                   target = frightenedTarget;
    }
public:
    Clyde(const Level* lvl, sf::Vector2i gridPos, sf::Vector2i scatterPos, sf::Vector2i frightenedPos)
        : Ghost(Config::sprites::clyde, lvl, gridPos, scatterPos, frightenedPos) {}
    virtual ~Clyde() {}
};