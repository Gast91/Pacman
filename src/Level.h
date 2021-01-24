#pragma once
#include "Tile.h"

using TileGrid = std::vector<std::vector<std::unique_ptr<Tile>>>;

class Level : public sf::Drawable
{
private:
	const char* bgFile = "resources/sprites/background.png";

    TileGrid tileGrid;

	sf::Texture bgTexture;
	sf::Sprite background;

	sf::Vector2i pacmanPosition;
private:
	bool readLevel(std::string filePath);
public:
	Level();
	~Level();

	bool isWall(sf::Vector2i coords) const;
    bool isInaccessible(sf::Vector2i coords) const;
    bool isIntersection(sf::Vector2i coords) const;
    bool isTeleporter(sf::Vector2i coords) const;

	sf::Vector2i getPacmanPosition() const;
	void updatePacmanPosition(sf::Vector2i coords);
    sf::Vector2i teleport();

	void updateTiles(sf::Vector2i coords);

	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};

// something that keeps track of pacman's pos that pacman itself updates?