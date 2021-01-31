#pragma once
#include "Config.h"

class GhostObserver
{
public:
    virtual void updateState(GhostState gs) = 0;
    virtual void updateTarget(std::pair<sf::Vector2i, sf::Vector2i> pacMovement) = 0;
    virtual GhostState getState() const = 0;
    virtual bool isNearHome() const = 0;
    virtual sf::Vector2i getCoords() const = 0;

};

class PacmanObserver
{
public:
    virtual void teleport(int newX) = 0;
    virtual std::pair<sf::Vector2i, sf::Vector2i> getMovement() const = 0;
};

class Subject
{
protected:
    std::vector<GhostObserver*> observers;
public:
    virtual void registerObserver(GhostObserver* observer) { observers.push_back(observer); }
    virtual void notifyObservers(GhostState gs) = 0;
};