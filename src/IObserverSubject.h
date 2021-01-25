#pragma once
#include "Config.h"

class GhostObserver
{
public:
    virtual void updateState(GhostState gs) = 0;
    virtual GhostState getState() = 0;
    virtual bool isNearHome() = 0;
    virtual sf::Vector2f getPos() = 0;
};

class PacmanObserver
{
public:
    virtual void teleport(int newX) = 0;
    virtual sf::Vector2i getGridPos() = 0;
};

class Subject
{
public:
    virtual void registerObserver(GhostObserver* observer) = 0;
    virtual void registerPacman(PacmanObserver* pacObs) = 0;
    //virtual void removeObserver(GhostObserver* observer) = 0;
    virtual void notifyObserver(GhostObserver* observer, GhostState gs) = 0;
    virtual void notifyObservers(GhostState gs) = 0;
};