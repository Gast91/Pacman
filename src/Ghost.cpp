#include "Ghost.h"

Ghost::Ghost(const char* spritesheet, const Level* lvl, sf::Vector2i gridPos, sf::Vector2i scatterPos, sf::Vector2i frightenedPos)
    : Entity(spritesheet, lvl, gridPos), scatterTarget(scatterPos), frightenedTarget(frightenedPos)
{
    Ghost::huntedSpritesheet.loadFromFile(Config::sprites::hunted); // Error handling!

    velocity *= 0.5f;

#ifndef CLASSIC
	aStar = new AStar(lvl);    // MOVE OUTSIDE
#endif // !CLASSIC
}

Ghost::~Ghost() { delete aStar; }  // nope

void Ghost::updateAnimation(const sf::Vector2i direction)
{
    switch (state)
    {
    case Chase:
    case Scatter:
        sprite.setTexture(spriteSheet);
        Entity::updateAnimation(direction);
        break;
    case Frightened:
        sprite.setTexture(huntedSpritesheet);
        sprite.setTextureRect(huntedAnim.nextFright());  // meh
        break;
    case Dead:
        sprite.setTextureRect(huntedAnim.next(direction));
    default:
        break;
    }
}

void Ghost::changeDirection(const sf::Vector2i target)
{
    int bestDist = INT_MAX;
    sf::Vector2i bestDir;

    for (const auto& dir : { NORTH, WEST, SOUTH, EAST }) // HMMM what about east
    {
        if (dir != direction * -1 && !level->isWall(gridPosition + dir))  //reversing in the tunnel??
        {
            int dist = distance(target, gridPosition + dir);
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

        sf::Vector2f nodePos = coordsToPosition(gridPosition + direction);
        if (distance(nodePos, sprite.getPosition()) <= Config::ENTITY_SIZE / 2.0f) // bit less? in others as well
        {
            gridPosition += direction;
            sprite.setPosition(nodePos);
            inBetween = false;
        }
    }
    else
    {
        if (level->isIntersection(gridPosition))
        {
#ifdef CLASSIC
            changeDirection(target);
#else
            path.clear();
            aStar->getPath(path, gridPosition, target, direction);
            if (!path.empty() && path.size() > 2)
                direction = path.at(1)->gridPosition - gridPosition;
            else changeDirection(target);
#endif
            updateAnimation(direction);
        }
        inBetween = true;
    }
}

void Ghost::updateState(GhostState gs) { state = gs; }

void Ghost::updateTarget(std::pair<sf::Vector2i, sf::Vector2i> pacMovement)
{
    if      (state == Chase)   target = pacMovement.first;
    else if (state == Scatter) target = scatterTarget;
    else                       target = frightenedTarget;
}

GhostState Ghost::getState() { return state; }

bool Ghost::isNearHome()
{
    return distance(coordsToPosition(frightenedTarget), sprite.getPosition()) <= Config::ENTITY_SIZE / 2.0f ? true : false;
}

sf::Vector2i Ghost::getCoords() { return gridPosition; }

sf::VertexArray Ghost::debugLines()
{
    sf::VertexArray lines(sf::LineStrip, path.size());
    for (int i = 0; i < path.size(); i++)
    {
        lines[i].position = sf::Vector2f(path[i]->gridPosition.x * Config::ENTITY_SIZE + Config::SCALED_OFFSET,
                                         path[i]->gridPosition.y * Config::ENTITY_SIZE + Config::SCALED_OFFSET);
        lines[i].color = sf::Color::Red;
    }
    return lines;
}