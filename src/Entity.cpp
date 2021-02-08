#include "Entity.h"

Entity::Entity(const char* spriteSheet, sf::Vector2i gridPos) 
    : gridPosition(gridPos)//, spriteSheet(Util::loadTexture(spriteSheet))
{
    movAnim.setTexture(spriteSheet);
    sprite.setTexture(movAnim.getTexture());
    sprite.scale(Config::SCALE, Config::SCALE);
    sprite.setPosition(Config::ENTITY_SIZE * gridPosition.x, Config::ENTITY_SIZE * gridPosition.y);
}

void Entity::updateAnimation(const sf::Vector2i direction) { sprite.setTextureRect(movAnim.next(direction)); }

void Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const { target.draw(sprite); }