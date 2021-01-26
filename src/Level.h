#pragma once
#include <array>
#include "Tile.h"
#include "Timer.h"
#include "IObserver.h"


using TileGrid = std::array<std::array<std::unique_ptr<Tile>, Config::COLS>, Config::ROWS>;

class Level : public sf::Drawable, public Subject
{
private:
	const char* bgFile = "resources/sprites/background.png";

    TileGrid tileGrid;

	sf::Texture bgTexture;
	sf::Sprite background;

    Timer scatterChaseTimer;
    Timer huntedTimer;

    PacmanObserver* pacmanObserver;

    bool over = false;

	bool readLevel(std::string filePath);
    bool shouldScatter();
public:
	Level();
	~Level();

    void registerPacman(PacmanObserver* pacObs);
    virtual void notifyObservers(GhostState gs) override;

	bool isWall(sf::Vector2i coords) const;
    bool isInaccessible(sf::Vector2i coords) const;
    bool isIntersection(sf::Vector2i coords) const;

    bool gameOver() const;

    void update();

	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};