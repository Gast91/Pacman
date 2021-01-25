#pragma once
#include <deque>
#include <list>

#include "Level.h"

class AStar // generic pathfind class with an AStar function?
{
private:
    const Level* level;

    void generateChild(const std::unique_ptr<Node>& current, const sf::Vector2i& newLoc, std::vector<std::unique_ptr<Node>>& children);
public:
	AStar(const Level* lvl);

    void getPath(std::deque<std::unique_ptr<Node>>& path, const sf::Vector2i& startPos, const sf::Vector2i& endPos, const sf::Vector2i& currentDir);
};