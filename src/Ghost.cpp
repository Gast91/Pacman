#include "Ghost.h"

Ghost::Ghost(const char* spritesheet, const AStar* astar, sf::Vector2i gridPos, sf::Vector2i scatterPos, sf::Vector2i frightenedPos = INVALID)
    : Entity(spritesheet, gridPos), aStar(astar), scatterTarget(scatterPos), frightenedTarget(frightenedPos == INVALID ? gridPos : frightenedPos)
{
    huntedAnim.setTexture(Config::sprites::hunted);
    velocity *= 0.5f;
    pathLines.setPrimitiveType(sf::LineStrip);
}

void Ghost::updateAnimation(const sf::Vector2i& direction)
{
    switch (state)
    {
    case GhostState::Waiting:
    case GhostState::Chase:
    case GhostState::Scatter:
        setTexture(movAnim.getTexture());
        Entity::updateAnimation(direction);
        break;
    case GhostState::Frightened:
        setTexture(huntedAnim.getTexture());
        setTextureRect(huntedAnim.next());
        break;
    case GhostState::Dead:
        setTextureRect(eatenAnim.next(direction));
        break;
    case GhostState::Paused:
    default: break;
    }
}

void Ghost::changeDirection(const sf::Vector2i& target)
{
    int bestDist = INT_MAX;
    sf::Vector2i bestDir;

    for (const auto& dir : { NORTH, WEST, SOUTH, EAST })
    {
        if (dir != direction * -1 && !aStar->isWall(gridPosition + dir))
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
    if (state == GhostState::Waiting || state == GhostState::Paused) return;
    if (inBetween)
    {
        sf::Sprite::move(direction.x * velocity, direction.y * velocity);

        sf::Vector2f nodePos = Util::coordsToPosition(gridPosition + direction);
        if (Util::distance(nodePos, getPosition()) <= Config::ENTITY_SIZE / 2.0f) //sf::Sprite::getGlobalBounds().contains(nodePos)
        {
            gridPosition += direction;
            setPosition(nodePos);
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

GhostState Ghost::getState() const { return state; }

bool Ghost::isNearHome() const { return sf::Sprite::getGlobalBounds().contains(Util::coordsToPosition(frightenedTarget)); }

sf::Vector2i Ghost::getCoords() const { return gridPosition; }

sf::FloatRect Ghost::getGlobalBounds() const { return sf::Sprite::getGlobalBounds(); }

void Ghost::reset()
{
    state = GhostState::Chase;
    gridPosition = initialGridPos;
    setPosition(Config::ENTITY_SIZE * gridPosition.x, Config::ENTITY_SIZE * gridPosition.y);
    setTexture(movAnim.getTexture());
    path.clear();
}

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