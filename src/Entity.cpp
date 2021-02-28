#include "Entity.h"

Entity::Entity(const char* spriteSheet, sf::Vector2i gridPos) 
    : gridPosition(gridPos), initialGridPos(gridPos)
{
    movAnim.setTexture(spriteSheet);
    setTexture(movAnim.getTexture());
    scale(Config::SCALE, Config::SCALE);
    setPosition(Config::ENTITY_SIZE * gridPosition.x, Config::ENTITY_SIZE * gridPosition.y + Config::TOP_BANNER_H);
}

void Entity::updateAnimation(const sf::Vector2i& direction) { setTextureRect(movAnim.next(direction)); }