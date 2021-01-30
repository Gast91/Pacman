#pragma once
#include <deque>
#include <list>

#include "Level.h"

class AStar
{
private:
    const Level* level;

    void generateChild(const std::unique_ptr<Node>& current, const sf::Vector2i& newLoc, std::vector<std::unique_ptr<Node>>& children) const;
public:
	AStar(const Level* lvl);

    bool isWall(const sf::Vector2i coords) const;
    bool isIntersection(const sf::Vector2i coords) const;
    void getPath(std::deque<std::unique_ptr<Node>>& path, const sf::Vector2i& startPos, const sf::Vector2i& endPos, const sf::Vector2i& currentDir) const;
};