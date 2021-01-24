#pragma once
#include "SFML/Graphics.hpp"
#include "Config.h"

class Level;

struct Node
{
    sf::Vector2i gridPosition;
    int f = 0, g = 0, h = 0;	 // score, cost, heuristic   
    Node* parent = nullptr;

    Node() {}
    Node(sf::Vector2i pos, Node* p = nullptr) : gridPosition(pos), parent(p) {}
    Node(Node* other) : gridPosition(other->gridPosition), parent(other->parent), f(other->f), g(other->g), h(other->h) {}

    inline bool operator== (const Node& rhs) { return gridPosition.x == rhs.gridPosition.x && gridPosition.y == rhs.gridPosition.y; }
};

class Tile : public sf::Drawable
{
private:
	bool wall   = false;
	bool dot    = false;
	bool bigDot = false;
    bool intersection = false;
    bool checked = false;
    bool teleporter = false;
    bool gate = false;
	sf::CircleShape dotSprite; // can get world position from here
public:
	Tile();
    Tile(sf::Vector2i gridCoords, sf::Vector2f pos, int type, Node* p = nullptr);
	~Tile();

    Node node;  // dont like it public

	bool hasADot() const;

	void setEaten();
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    friend Level;
};