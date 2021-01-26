#include <fstream>
#include "Level.h"

Level::Level()
{
	readLevel("resources/grid.txt"); // this returns successful load or not - use it
	background.scale(Config::SCALE, Config::SCALE);
	if (bgTexture.loadFromFile(bgFile))
		background.setTexture(bgTexture);

    // if everything is okay here
    scatterChaseTimer.startTimer();
}

Level::~Level() {}

void Level::registerPacman(PacmanObserver* pacObs) { pacmanObserver = pacObs; }
void Level::notifyObservers(GhostState gs) { for (auto& observer : observers) observer->updateState(gs); }

bool Level::readLevel(std::string filePath)
{
	std::ifstream infile;
	infile.open(filePath);

	if (!infile)
	{
		// error message and abort everything also (also for other resources)
		return false;
	}

	int token;
    for (int y = 0; y < Config::COLS; ++y)
    {
        for (int x = 0; x < Config::ROWS; ++x)
        {
            if (!(infile >> token)) return false;
            tileGrid[x][y].reset(new Tile({ x, y }, { x * Config::ENTITY_SIZE + Config::SCALED_OFFSET, y * Config::ENTITY_SIZE + Config::SCALED_OFFSET }, token));
        }
    }
    return true;
}

bool Level::isWall(sf::Vector2i coords) const { return tileGrid[coords.x][coords.y]->wall; }

bool Level::isInaccessible(sf::Vector2i coords) const { return tileGrid[coords.x][coords.y]->wall || tileGrid[coords.x][coords.y]->gate; }

bool Level::isIntersection(sf::Vector2i coords) const
{
    if (!tileGrid[coords.x][coords.y]->checked)
    {
        for (const auto dir : { NORTH, SOUTH, EAST, WEST })
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

bool Level::gameOver() const { return over; }

//sf::Vector2i Level::getPacmanPosition() const { return pacmanObserver->getGridPos(); }

bool Level::shouldScatter()
{
    // 0-7 (SC) --> 7-27 (CH) --> 27-34 (SC) --> 34-54 (CH) --> 54-59 (SC) --> 59-79 (CH) --> 79-84 (SC) --> CHASE FOREVER*
    double timeEllapsed = scatterChaseTimer.msEllapsed() / 1000.0;
    if    ((timeEllapsed > 0.0  && timeEllapsed <= 7.0)
        || (timeEllapsed > 27.0 && timeEllapsed <= 34.0)
        || (timeEllapsed > 54.0 && timeEllapsed <= 59.0)
        || (timeEllapsed > 79.0 && timeEllapsed <= 84.0)) return true;
    else return false;
}

void Level::update()
{
    // Update pacman's position on the grid
    //sf::Vector2i pacmanCoords = pacmanObserver->getGridPos();
    const auto& pacmanCoords = pacmanObserver->getMovement().first;
    // If pacman should teleport, notify and provide new grid position (currently only for X)
    if (tileGrid[pacmanCoords.x][pacmanCoords.y]->teleporter) pacmanObserver->teleport(pacmanCoords.x == 0 ? Config::ROWS - 1 : 0);
    // If pacman ate a big dot, notify ghosts to run
    if (tileGrid[pacmanCoords.x][pacmanCoords.y]->bigDot)
    {
        notifyObservers(GhostState::Frightened);
        scatterChaseTimer.pauseTimer();
        huntedTimer.startTimer();
    }
    // Update underlying tile if pacman ate a dot
    if (tileGrid[pacmanCoords.x][pacmanCoords.y]->hasADot()) tileGrid[pacmanCoords.x][pacmanCoords.y]->setEaten();

    // Check if frightened timer expired, notify ghosts and resume scatter/chase timer
    if (huntedTimer.isRunning() && huntedTimer.msEllapsed() / 1000.0 >= 10.0)  // 10seconds duration?
    {
        huntedTimer.stopTimer();
        scatterChaseTimer.resumeTimer();
        notifyObservers(shouldScatter() ? GhostState::Scatter : GhostState::Chase);
    }
    else if (!huntedTimer.isRunning()) notifyObservers(shouldScatter() ? GhostState::Scatter : GhostState::Chase);

    // Update each ghost's state based on its current state, distance to pacman or distance to its home
    for (auto& observer : observers)
    {
        GhostState obsState = observer->getState();

        // Calculate distance to pacman
        float distanceToPacman = distance(coordsToPosition(pacmanCoords), coordsToPosition(observer->getCoords()));

        // Pacman ate this ghost
        if (obsState == GhostState::Frightened && distanceToPacman <= Config::ENTITY_SIZE / 2.0f) observer->updateState(GhostState::Dead);
        // This ghost came back to life
        else if (obsState == GhostState::Dead && observer->isNearHome()) observer->updateState(GhostState::Scatter);
        // This ghost ate pacman
        else if ((obsState == GhostState::Chase || obsState == GhostState::Scatter) && distanceToPacman <= Config::ENTITY_SIZE / 2.0f)
            over = true;  // LIFE COUNTER, RESTART, TIMERS, ETC ETC

        observer->updateTarget(pacmanObserver->getMovement());
    }
}

void Level::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(background);
    for (auto& col : tileGrid)
    {
        for (auto& tile : col) target.draw(*tile);
    }
}