#include "Entity.h"

Entity::Entity(const char* spriteSheet, const Level* lvl, sf::Vector2i gridPos) : level(lvl), gridPosition(gridPos)
{
	if (Entity::spriteSheet.loadFromFile(spriteSheet))
		sprite.setTexture(Entity::spriteSheet);
	loadAnimations();

    // Scale the sprite and position it in the world according to its grid position
    sprite.scale(Config::SCALE, Config::SCALE);
    sprite.setPosition(Config::ENTITY_SIZE * gridPosition.x, Config::ENTITY_SIZE * gridPosition.y);
}

Entity::~Entity() {}

void Entity::loadAnimations()
{
	for (int i = 0; i < 2; ++i)
	{
		movAnim.right[i] = { i * SIZE,  0,        SIZE, SIZE };
		movAnim.left[i]  = { i * SIZE,  SIZE,     SIZE, SIZE };
		movAnim.up[i]    = { i * SIZE,  2 * SIZE, SIZE, SIZE };
		movAnim.down[i]  = { i * SIZE,  3 * SIZE, SIZE, SIZE };
	}

	sprite.setTextureRect(movAnim.right[0]);
}

void Entity::updateAnimation(const sf::Vector2i direction) { sprite.setTextureRect(movAnim.next(direction)); }

void Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const { target.draw(sprite); }