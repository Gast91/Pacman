#include "Ghost.h"

Ghost::Ghost(const char* spritesheet, const AStar* astar, sf::Vector2i gridPos, sf::Vector2i scatterPos, sf::Vector2i frightenedPos)
    : Entity(spritesheet, gridPos), aStar(astar), scatterTarget(scatterPos), frightenedTarget(frightenedPos), huntedSpritesheet(Util::loadTexture(Config::sprites::hunted))
{
    velocity *= 0.5f;
    pathLines.setPrimitiveType(sf::LineStrip);
}

void Ghost::updateAnimation(const sf::Vector2i direction)
{
    switch (state)
    {
    case GhostState::Chase:
    case GhostState::Scatter:
        sprite.setTexture(*spriteSheet);
        Entity::updateAnimation(direction);
        break;
    case GhostState::Frightened:
        sprite.setTexture(*huntedSpritesheet);
        sprite.setTextureRect(huntedAnim.nextFright());  // meh
        break;
    case GhostState::Dead:
        sprite.setTextureRect(huntedAnim.next(direction));
        break;
    default: break;
    }
}

void Ghost::changeDirection(const sf::Vector2i target)
{
    int bestDist = INT_MAX;
    sf::Vector2i bestDir;

    for (const auto& dir : { NORTH, WEST, SOUTH, EAST }) // HMMM what about east
    {
        if (dir != direction * -1 && !aStar->isWall(gridPosition + dir))  //reversing in the tunnel??
        {
            int dist = Util::distance(target, gridPosition + dir);
            if (dist < bestDist)
            {
                bestDist = dist;
                bestDir = dir;
            }
        }
    }
    direction = bestDir;
}

void Ghost::move()
{
    if (inBetween)
    {
        sprite.move(direction.x * velocity, direction.y * velocity);

        sf::Vector2f nodePos = Util::coordsToPosition(gridPosition + direction);
        if (Util::distance(nodePos, sprite.getPosition()) <= Config::ENTITY_SIZE / 2.0f) // bit less? in others as well
        {
            gridPosition += direction;
            sprite.setPosition(nodePos);
            inBetween = false;
        }
    }
    else
    {
        if (aStar->isIntersection(gridPosition))
        {
            path.clear();
            aStar->getPath(path, gridPosition, target, direction);
            if (!path.empty() && path.size() > 2)
                direction = path.at(1)->gridPosition - gridPosition;
            else changeDirection(target);

            updateAnimation(direction);
        }
        inBetween = true;
    }
}

void Ghost::updateState(GhostState gs) { state = gs; }

void Ghost::updateTarget(std::pair<sf::Vector2i, sf::Vector2i> pacMovement)
{
    if      (state == GhostState::Chase)   target = pacMovement.first;
    else if (state == GhostState::Scatter) target = scatterTarget;
    else                                   target = frightenedTarget;
}

GhostState Ghost::getState() { return state; }

bool Ghost::isNearHome()
{
    return Util::distance(Util::coordsToPosition(frightenedTarget), sprite.getPosition()) <= Config::ENTITY_SIZE / 2.0f ? true : false;
}

sf::Vector2i Ghost::getCoords() { return gridPosition; }

const sf::VertexArray& Ghost::debugLines(const sf::Color color)
{
    pathLines.clear();
    pathLines.resize(path.size());
    for (int i = 0; i < path.size(); ++i)
    {
        pathLines[i].position = sf::Vector2f(path[i]->gridPosition.x * Config::ENTITY_SIZE + Config::SCALED_OFFSET,
                                             path[i]->gridPosition.y * Config::ENTITY_SIZE + Config::SCALED_OFFSET);
        pathLines[i].color = color;
    }
    return pathLines;
}