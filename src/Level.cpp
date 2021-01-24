#include <fstream>
#include "Level.h"

Level::Level()
{
	readLevel("resources/grid.txt"); // this returns successful load or not - use it
	background.scale(Config::SCALE, Config::SCALE);
	if (bgTexture.loadFromFile(bgFile))
		background.setTexture(bgTexture);
}

Level::~Level() {}

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
    tileGrid.resize(Config::ROWS);
    for (int y = 0; y < Config::COLS; ++y)
    {
        for (int x = 0; x < Config::ROWS; ++x)
        {
            tileGrid[x].resize(Config::COLS);
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

bool Level::isTeleporter(sf::Vector2i coords) const { return tileGrid[coords.x][coords.y]->teleporter; }

sf::Vector2i Level::getPacmanPosition() const { return pacmanPosition; }

void Level::updatePacmanPosition(sf::Vector2i coords) { pacmanPosition = coords; }

sf::Vector2i Level::teleport() 
{ 
    pacmanPosition.x = pacmanPosition.x == 0 ? Config::ROWS - 1 : 0; 
    return pacmanPosition;
}

void Level::updateTiles(sf::Vector2i coords) { if (tileGrid[coords.x][coords.y]->hasADot()) tileGrid[coords.x][coords.y]->setEaten(); }

void Level::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(background);
    for (auto& col : tileGrid)
    {
        for (auto& tile : col) target.draw(*tile);
    }
}