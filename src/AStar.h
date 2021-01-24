#pragma once
#include <deque>
#include <list>

#include "Level.h"

class AStar // generic pathfind class with an AStar function?
{
private:
    Level* level;

	//Node nodeGrid[Config::ROWS][Config::COLS];
	
	//void generateChild(sf::Vector2i& currentPos, sf::Vector2i location, std::vector<Node>& children);

    void GenerateChild(const std::unique_ptr<Node>& current, const sf::Vector2i& newLoc, std::vector<std::unique_ptr<Node>>& children);

	//void resetGrid();
public:
	AStar(Level* lvl);

    void getPath(std::deque<std::unique_ptr<Node>>& path, const sf::Vector2i& startPos, const sf::Vector2i& endPos, const sf::Vector2i& currentDir);
};