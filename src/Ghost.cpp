#include "Ghost.h"

Ghost::Ghost(const char* spritesheet, const char* huntedSpritesheet, Level* lvl, sf::Vector2i gridPos, sf::Vector2i scatterPos, sf::Vector2i frightenedPos)
    : Entity(spritesheet, lvl, gridPos), scatterTarget(scatterPos), frightenedTarget(frightenedPos)
{
    Ghost::huntedSpritesheet.loadFromFile(huntedSpritesheet); // Error handling!

    loadAnimations();

    velocity *= 0.5f;

#ifndef CLASSIC
	aStar = new AStar(lvl);
#endif // !CLASSIC
}

Ghost::~Ghost() { delete aStar; }  // nope

void Ghost::loadAnimations()  // can it be improved??
{
    Entity::loadAnimations();
    sprite.setTexture(huntedSpritesheet);

    huntedAnim.frightened[0] = { 0   , 0   , SIZE, SIZE };
    huntedAnim.frightened[1] = { SIZE, 0   , SIZE, SIZE };
    huntedAnim.frightened[2] = { 0   , SIZE, SIZE, SIZE };
    huntedAnim.frightened[3] = { SIZE, SIZE, SIZE, SIZE };

    huntedAnim.deadRight = { 0   , 2 * SIZE, SIZE, SIZE };
    huntedAnim.deadLeft  = { SIZE, 2 * SIZE, SIZE, SIZE };
    huntedAnim.deadUp =    { 0   , 3 * SIZE, SIZE, SIZE };
    huntedAnim.deadDown =  { SIZE, 3 * SIZE, SIZE, SIZE };

    sprite.setTexture(spriteSheet);
}

void Ghost::updateAnimation(const sf::Vector2i direction)
{
    switch (state)
    {
    case Chase:
    case Scatter:
        Entity::updateAnimation(direction);
        break;
    case Frightened:
        sprite.setTextureRect(huntedAnim.frightened[huntedAnim.next()]);
        break;
    case Dead:  // Something to be done here?
        if (direction == NORTH)
            sprite.setTextureRect(huntedAnim.deadUp);
        else if (direction == SOUTH)
            sprite.setTextureRect(huntedAnim.deadDown);
        else if (direction == WEST)
            sprite.setTextureRect(huntedAnim.deadLeft);
        else if (direction == EAST)
            sprite.setTextureRect(huntedAnim.deadRight);
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
        if (dir != direction * -1 && !level->isWall(gridPosition + dir))
        {
            sf::Vector2i distanceVector = target - (gridPosition + dir);
            int distance = distanceVector.x * distanceVector.x + distanceVector.y * distanceVector.y;
            if (distance < bestDist)
            {
                bestDist = distance;
                bestDir = dir;
            }
        }
    }
    direction = bestDir;
}

void Ghost::move()  // this needs MAJOR REFACTOR  -- also this movement is for one type of ghost - use the pathfinding factory for the ghost personalities?
{
    if (inBetween)
    {
        sprite.move(direction.x * velocity, direction.y * velocity);

        sf::Vector2i next = gridPosition + direction;  // some calcs happen more times than needed here
        sf::Vector2f nodePos = { next.x * Config::ENTITY_SIZE, next.y * Config::ENTITY_SIZE };  // get them from the level directly?
        sf::Vector2f distanceVector = nodePos - sprite.getPosition();
        float distance = distanceVector.x * distanceVector.x + distanceVector.y * distanceVector.y;
        if (distance <= Config::ENTITY_SIZE / 2.0f) // ??
        {
            gridPosition = next;   // ugh
            sprite.setPosition(nodePos);
            inBetween = false;
        }
    }
    else
    {
        if (level->isIntersection(gridPosition))
        {
            sf::Vector2i target;       // make it better a bit?
            if      (state == Chase)   target = level->getPacmanPosition();
            else if (state == Scatter) target = scatterTarget;
            else                       target = frightenedTarget;
#ifdef CLASSIC
            changeDirection(target);
#else
            aStar->getPath(path, gridPosition, target, direction);
            if (!path.empty() && path.size() > 2)
            {
                sf::Vector2i next = path.at(1)->gridPosition;
                direction = next - gridPosition;
            }
            else if (state == Scatter || state == Frightened || state == Dead) changeDirection(target);
            //else I ATE PACKMAN?!
#endif
            updateAnimation(direction);
        }
        inBetween = true;
    }

	// check states
	// move ghost accordingly
	// update animations
}

void Ghost::debugTargeting()
{
    state = static_cast<GhostState>((state + 1) % 4);
    switch (state)
    {
    case Chase:
    case Scatter:
        sprite.setTexture(spriteSheet);
        break;
    case Frightened:
    case Dead:
        sprite.setTexture(huntedSpritesheet);
        break;
    default:
        break;
    }
}
