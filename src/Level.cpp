#include "Level.h"

Level::Level() :
    tileGrid([]() {
        std::ifstream infile;
        infile.open(Config::grid);

        if (!infile) throw std::exception("Cannot find grid file");

        TileGrid grid{};
        int token;
        for (int y = 0; y < Config::COLS; ++y)
        {
            for (int x = 0; x < Config::ROWS; ++x)
            {
                if (!(infile >> token)) throw std::exception("Unable to read grid file");
                grid[x][y].reset(new Tile({ x, y }, { x * Config::ENTITY_SIZE + Config::SCALED_OFFSET, y * Config::ENTITY_SIZE + Config::SCALED_OFFSET }, token));
            }
        }
        return grid; 
    }()),
    bgTexture(Util::loadTexture(Config::sprites::bckgnd)),
        background(Util::createSprite(*bgTexture))
{}

void Level::registerPacman(PacmanObserver* pacObs) { pacmanObserver = pacObs; }

void Level::notifyObservers(const GhostState gs) { for (auto& observer : observers) observer->updateState(gs); }

int Level::getEaten() const { return dotsEaten; }

bool Level::isWall(const sf::Vector2i coords) const { return tileGrid[coords.x][coords.y]->type == TileType::Wall; }

bool Level::isInaccessible(const sf::Vector2i coords) const { return tileGrid[coords.x][coords.y]->type == TileType::Wall || tileGrid[coords.x][coords.y]->type == TileType::Gate; }

bool Level::isIntersection(const sf::Vector2i coords) const
{
    if (!tileGrid[coords.x][coords.y]->checked)
    {
        for (const auto& dir : { NORTH, SOUTH, EAST, WEST })
        {
            sf::Vector2i neighbor = coords + dir;
            if (neighbor.x < 0 || neighbor.y < 0 || neighbor.x > Config::ROWS - 1 || neighbor.y > Config::COLS - 1) continue;
            if (isWall(neighbor)) continue;
            tileGrid[coords.x][coords.y]->checked = true;
            tileGrid[coords.x][coords.y]->intersection = true;
            break;
        }
    }
    return tileGrid[coords.x][coords.y]->intersection;
}

bool Level::gameOver()   const { return over; }
bool Level::isPaused()   const { return paused; }
bool Level::hasStarted() const { return start; }

bool Level::shouldScatter() const
{
    // 0-7 (SC) --> 7-27 (CH) --> 27-34 (SC) --> 34-54 (CH) --> 54-59 (SC) --> 59-79 (CH) --> 79-84 (SC) --> CHASE FOREVER*
    double timeEllapsed = scatterChaseTimer.msEllapsed() / 1000.0;
    if    ((timeEllapsed > 0.0  && timeEllapsed <= 7.0)
        || (timeEllapsed > 27.0 && timeEllapsed <= 34.0)
        || (timeEllapsed > 54.0 && timeEllapsed <= 59.0)
        || (timeEllapsed > 79.0 && timeEllapsed <= 84.0)) return true;
    else return false;
}

// this at some point should also modify speeds and other variables that are based on level
void Level::nextLevel()
{
    pacmanObserver->reset();
    for (auto& observer : observers) observer->reset();
    for (auto& cols : tileGrid) { for (auto& tile : cols) tile->reset(); }
    score.reset(); // just the display not the score itself
    dotsEaten = 0;
    ++collectible;
    start = false;
}

void Level::reset()
{
    pacmanObserver->reset();
    for (auto& observer : observers) observer->reset();
    score.reset(); // just the display not the score itself
    over = false;
    --lives;
    start = false;
}

void Level::restart()
{
    pacmanObserver->reset();
    for (auto& observer : observers) observer->reset();
    for (auto& cols : tileGrid) { for (auto& tile : cols) tile->reset(); }
    over = false;
    lives.reset();
    score.restart();
    dotsEaten = 0;
    start = false;
}

void Level::begin()
{
    start = true;
    over = false;
    pacmanObserver->start();
    score.start();
    scatterChaseTimer.startTimer();
}

void Level::update()
{
    if (!start) return;
    if (paused) { paused = false; huntedTimer.resumeTimer(); }  // resume timers that SHOULD BE RESUMED - PAUSE will happen for fruits also
    if (over && pacmanObserver->playDeath() && lives.getLives() > 0) reset();
    else if (over && pacmanObserver->playDeath() && lives.getLives() == 0) restart();
    else if (over) return;

    // If pacman ate all the dots, we go to the next level
    if (dotsEaten == Config::TOTAL_DOTS) { nextLevel(); return; }  // what is the maximum level that pacman wins??

    // Update pacman's position on the grid
    const auto pacmanCoords = pacmanObserver->getMovement().first;
    // If pacman should teleport, notify and provide new grid position (currently only for X)
    if (tileGrid[pacmanCoords.x][pacmanCoords.y]->type == TileType::Teleporter) pacmanObserver->teleport(pacmanCoords.x == 0 ? Config::ROWS - 1 : 0);
    // Pacman's grid position contains a dot
    if (tileGrid[pacmanCoords.x][pacmanCoords.y]->hasADot())
    {
        if (tileGrid[pacmanCoords.x][pacmanCoords.y]->type == TileType::BigDot)
        {
            notifyObservers(GhostState::Frightened);
            scatterChaseTimer.pauseTimer();
            huntedTimer.startTimer();
        }
        tileGrid[pacmanCoords.x][pacmanCoords.y]->setEaten();
        ++dotsEaten;
        score += tileGrid[pacmanCoords.x][pacmanCoords.y]->getValue();
        // Pacman 'earns' a life every 10k points
        if (score.overThreshold()) ++lives;
    }

    // Check whether to spawn or consume a collectible
    if (collectible.isVisible() && pacmanCoords == Config::FRUIT_COORDS) score += collectible.eat();
    else if (dotsEaten == 70 || dotsEaten == 170)  collectible.spawnCollectible();  // paused = true; AND TIMERS! make a function that pauses timers that should be paused?

    // Check if frightened timer expired, notify ghosts and resume scatter/chase timer
    if (huntedTimer.isRunning() && huntedTimer.msEllapsed() / 1000.0 >= 10.0)  // 10seconds duration?
    {
        huntedTimer.stopTimer();
        scatterChaseTimer.resumeTimer();
        notifyObservers(shouldScatter() ? GhostState::Scatter : GhostState::Chase);
        score.resetGhostPoints();
    }
    else if (!huntedTimer.isRunning()) notifyObservers(shouldScatter() ? GhostState::Scatter : GhostState::Chase);

    // Update each ghost's state based on its current state, distance to pacman or distance to its home
    for (auto& observer : observers)
    {
        GhostState obsState = observer->getState();

        // Is there a collision with pacman?
        bool colliding = observer->getGlobalBounds().intersects(pacmanObserver->getGlobalBounds());

        // Pacman ate this ghost
        if (obsState == GhostState::Frightened && colliding) 
        { 
            observer->updateState(GhostState::Dead); 
            score.displayGhostPoints(Util::coordsToPosition(observer->getCoords()));
            // Pacman 'earns' a life every 10k points
            if (score.overThreshold()) ++lives;
            paused = true;
            huntedTimer.pauseTimer();
        }
        // This ghost came back to life
        else if (obsState == GhostState::Dead && observer->isNearHome()) observer->updateState(huntedTimer.isRunning() ? GhostState::Frightened : GhostState::Scatter);
        // This ghost ate pacman
        else if ((obsState == GhostState::Chase || obsState == GhostState::Scatter) && colliding)
        {
            over = true;
            notifyObservers(GhostState::Paused);
        }

        // Notify observers of pacman's new position/direction
        observer->updateTarget(pacmanObserver->getMovement());
    }
}

void Level::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(*background);
    for (auto& col : tileGrid) { for (auto& tile : col) target.draw(*tile); }
    target.draw(lives);
    target.draw(score);
    target.draw(collectible);
}