#pragma once
#include <fstream>
#include "Tile.h"
#include "Timer.h"
#include "IObserver.h"
#include "Banner.h"
#include "SoundManager.h"

using TileGrid = std::array<std::array<std::unique_ptr<Tile>, Config::COLS>, Config::ROWS>;

class Level : public sf::Drawable, public Subject
{
private:
    TileGrid tileGrid;

    const std::unique_ptr<sf::Texture> bgTexture;
    const std::unique_ptr<sf::Sprite> background;

    Timer scatterChaseTimer;
    Timer huntedTimer;

    ScoreDisplay score;
    SoundManager soundManager;
    Lives lives;
    Collectible collectible;
    PacmanObserver* pacmanObserver;

    bool over   = false;  // pacman died (might have lives remaining)
    bool paused = false;  // pacman ate a ghost, game paused to display pts
    bool start  = false;  // game waiting for user input to start
    int dotsEaten = 0;

    bool shouldScatter() const;
    void nextLevel();   // Reset game and increase level
    void reset();       // Reset level - pacman died
    void restart();     // Restart the whole game

    void pause();
    void resume();
public:
    Level();

    void registerPacman(PacmanObserver* pacObs);
    virtual void notifyObservers(const GhostState gs) override;

    bool isWall(const sf::Vector2i coords) const;
    bool isInaccessible(const sf::Vector2i coords) const;
    bool isIntersection(const sf::Vector2i coords) const;

    bool gameOver()   const;
    bool isPaused()   const;
    bool hasStarted() const;
    int  getEaten()   const;

    void update();
    void begin();
    void adjustSound(const float vol);
    void toggleSound(const sf::Vector2f& mousePos);
    void toggleSound();

    void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};