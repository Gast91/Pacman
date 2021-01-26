#include "Entity.h"

Entity::Entity(const char* spriteSheet, const Level* lvl, sf::Vector2i gridPos) : level(lvl), gridPosition(gridPos)
{
	if (Entity::spriteSheet.loadFromFile(spriteSheet))
		sprite.setTexture(Entity::spriteSheet);

    // Scale the sprite and position it in the world according to its grid position
    sprite.scale(Config::SCALE, Config::SCALE);
    sprite.setPosition(Config::ENTITY_SIZE * gridPosition.x, Config::ENTITY_SIZE * gridPosition.y);
}

Entity::~Entity() {}

void Entity::updateAnimation(const sf::Vector2i direction) { sprite.setTextureRect(movAnim.next(direction)); }

void Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const { target.draw(sprite); }