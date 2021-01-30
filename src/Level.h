#pragma once
#include <array>
#include <fstream>

#include "Tile.h"
#include "Timer.h"
#include "IObserver.h"

using TileGrid = std::array<std::array<std::unique_ptr<Tile>, Config::COLS>, Config::ROWS>;

class Level : public sf::Drawable, public Subject
{
private:
    TileGrid tileGrid;

    const std::unique_ptr<sf::Texture> bgTexture;
    const std::unique_ptr<sf::Sprite> background;

    Timer scatterChaseTimer;
    Timer huntedTimer;

    PacmanObserver* pacmanObserver;

    bool over = false;

    bool shouldScatter() const;
public:
	Level();

    void registerPacman(PacmanObserver* pacObs);
    virtual void notifyObservers(const GhostState gs) override;

	bool isWall(const sf::Vector2i coords) const;
    bool isInaccessible(const sf::Vector2i coords) const;
    bool isIntersection(const sf::Vector2i coords) const;

    bool gameOver() const;

    void update();

	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};