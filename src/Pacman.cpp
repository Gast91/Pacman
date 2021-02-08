#include "Pacman.h"

Pacman::Pacman(const Level* lvl, sf::Vector2i gridPos)
	: Entity(Config::sprites::pacman, gridPos), level(lvl) { deathAnim.setTexture(Config::sprites::pacDeath); }//, deathSpritesheet(Util::loadTexture(Config::sprites::pacDeath)) {}

void Pacman::teleport(int newX)
{
    gridPosition.x = newX;
    sprite.setPosition(gridPosition.x * Config::ENTITY_SIZE, gridPosition.y * Config::ENTITY_SIZE);
}

std::pair<sf::Vector2i, sf::Vector2i> Pacman::getMovement() const { return std::make_pair(gridPosition, direction); }

void Pacman::changeDirection(const sf::Vector2i nxtTurn) { nextTurn = nxtTurn; }

void Pacman::move()
{
	// Pacman can turn without hitting a wall or the ghost house gates, so change its direction
	if (!level->isInaccessible(gridPosition + nextTurn)) { direction = nextTurn; }

	// Do nothing if Pacman is about to move into a wall or the ghost house gates
	if (level->isInaccessible(gridPosition + direction)) return;

	// Play Pacman animation according to the direction it's facing
    updateAnimation(direction);  // --> if dead we play dead anim...

	// Move Pacman and update its position on the level
	sprite.move(direction.x * velocity, direction.y * velocity);
    gridPosition += direction;
}