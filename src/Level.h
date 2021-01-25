#pragma once
#include "Tile.h"
#include "Timer.h"
#include "IObserverSubject.h"

using TileGrid = std::vector<std::vector<std::unique_ptr<Tile>>>;

class Level : public sf::Drawable, public Subject
{
private:
	const char* bgFile = "resources/sprites/background.png";

    TileGrid tileGrid;

	sf::Texture bgTexture;
	sf::Sprite background;

	sf::Vector2i pacmanCoords;
    Timer scatterChaseTimer;
    Timer huntedTimer;
private:
	bool readLevel(std::string filePath);

    std::vector<GhostObserver*> observers;
    PacmanObserver* pacmanObserver;

    bool shouldScatter();
public:
	Level();
	~Level();

    virtual void registerObserver(GhostObserver* observer) override;
    virtual void registerPacman(PacmanObserver* pacObs) override;
    virtual void notifyObservers(GhostState gs) override;
    virtual void notifyObserver(GhostObserver* observer, GhostState gs) override;

	bool isWall(sf::Vector2i coords) const;
    bool isInaccessible(sf::Vector2i coords) const;
    bool isIntersection(sf::Vector2i coords) const;
    bool isTeleporter(sf::Vector2i coords) const;

	sf::Vector2i getPacmanPosition() const;
	void updatePacmanPosition(sf::Vector2i coords);

    void update();

	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};