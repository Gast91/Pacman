#include "Pacman.h"

Pacman::Pacman(Level* lvl, sf::Vector2i gridPos) : Entity("resources/sprites/pacman_spritesheet.png", lvl, gridPos) {}
Pacman::~Pacman() {}

void Pacman::teleport(int newX)
{
    gridPosition.x = newX;
    sprite.setPosition(gridPosition.x * Config::ENTITY_SIZE, gridPosition.y * Config::ENTITY_SIZE);
}

sf::Vector2i Pacman::getGridPos() { return gridPosition; }

void Pacman::changeDirection(const sf::Vector2i nxtTurn) { nextTurn = nxtTurn; }

void Pacman::move()
{
	// Pacman can turn without hitting a wall or the ghost house gates, so change its direction
	if (!level->isInaccessible(gridPosition + nextTurn)) { direction = nextTurn; }

	// Do nothing if Pacman is about to move into a wall or the ghost house gates
	if (level->isInaccessible(gridPosition + direction)) return;

	// Play Pacman animation according to the direction it's facing
    updateAnimation(direction);

	// Move Pacman and update its position on the level
	sprite.move(direction.x * velocity, direction.y * velocity);
    gridPosition += direction;
}