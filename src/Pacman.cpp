#include "Pacman.h"

Pacman::Pacman(const Level* lvl, sf::Vector2i gridPos)
    : Entity(Config::sprites::pacman, gridPos), level(lvl) 
{ 
    deathAnim.setTexture(Config::sprites::pacDeath);
    setTexture(deathAnim.getTexture());
    setTextureRect(deathAnim.getFirst());
}

bool Pacman::playDeath()
{
    if (deathAnim.isDone()) return true;
    setTexture(deathAnim.getTexture());
    setTextureRect(deathAnim.next());
    return false;
}

void Pacman::teleport(int newX)
{
    gridPosition.x = newX;
    setPosition(gridPosition.x * Config::ENTITY_SIZE, gridPosition.y * Config::ENTITY_SIZE + Config::TOP_BANNER_H);
}

std::pair<sf::Vector2i, sf::Vector2i> Pacman::getMovement() const { return std::make_pair(gridPosition, direction); }
sf::FloatRect Pacman::getGlobalBounds() const { return sf::Sprite::getGlobalBounds(); }

void Pacman::reset()
{
    nextTurn = EAST;
    gridPosition = initialGridPos;
    setPosition(Config::ENTITY_SIZE * gridPosition.x, Config::ENTITY_SIZE * gridPosition.y + Config::TOP_BANNER_H);
    setTexture(deathAnim.getTexture());
    setTextureRect(deathAnim.getFirst());
    deathAnim.reset();
}

void Pacman::start() { setTexture(movAnim.getTexture()); }

void Pacman::changeDirection(const sf::Vector2i& nxtTurn) { nextTurn = nxtTurn; }

void Pacman::move()
{
    if (level->gameOver() || level->isPaused() || !level->hasStarted()) return;

    // Pacman can turn without hitting a wall or the ghost house gates, so change its direction
    if (!level->isInaccessible(gridPosition + nextTurn)) direction = nextTurn;

    // Do nothing if Pacman is about to move into a wall or the ghost house gates
    if (level->isInaccessible(gridPosition + direction)) return;

    // Play Pacman animation according to the direction it's facing
    updateAnimation(direction);

    // Move Pacman and update its position on the level
    sf::Sprite::move(direction.x * velocity, direction.y * velocity);
    gridPosition += direction;
}