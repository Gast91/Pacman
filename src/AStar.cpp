#include "AStar.h"

AStar::AStar(const Level* lvl) : level(lvl) {}

bool AStar::isWall(const sf::Vector2i coords) const { return level->isWall(coords); }

bool AStar::isIntersection(const sf::Vector2i coords) const { return level->isIntersection(coords); }

bool AStar::canExit(int eatLimit) const { return level->getEaten() >= eatLimit; }

void AStar::generateChild(const std::unique_ptr<Node>& current, const sf::Vector2i& newLoc, std::vector<std::unique_ptr<Node>>& children) const
{
    sf::Vector2i newPos = current->gridPosition + newLoc;
    if (newPos.x < 0 || newPos.y < 0 || newPos.x > Config::ROWS - 1 || newPos.y > Config::COLS - 1) return;
    if (level->isWall(newPos)) return;
    children.push_back(std::make_unique<Node>(new Node(newPos, current.get())));
}

void AStar::getPath(std::deque<std::unique_ptr<Node>>& path, const sf::Vector2i& startPos, const sf::Vector2i& endPos, const sf::Vector2i& currentDir) const
{
    std::deque<std::unique_ptr<Node>> openList;
    std::list<std::unique_ptr<Node>>  closedList;

    std::unique_ptr<Node> start(new Node(startPos));
    std::unique_ptr<Node> goal(new Node(endPos));

    openList.push_back(move(start));

    while (!openList.empty())
    {
        // Current becomes the first node in the open list
        std::unique_ptr<Node> current = move(openList.front());
        openList.pop_front();

        // End goal found when the node popped (current) is the goal
        if (current->gridPosition == goal->gridPosition)
        {
            std::unique_ptr<Node> next;
            while (current->parent)
            {
                // parent of current node becomes next
                next = std::make_unique<Node>(current->parent);
                // Add current to path
                path.push_front(move(current));
                // Current becomes next
                current = move(next);
            }
            // Last node aka the start node is pushed to the front
            path.push_front(move(current));
            return;
        }

        // Generate children of the current node
        std::vector<std::unique_ptr<Node>> children;
        for (const auto& dir : { NORTH, EAST, SOUTH, WEST }) generateChild(current, dir, children);

        for (auto& child : children)
        {
            // Calculate the f, g, and h values
            // The ghosts are not permitted to reverse direction, as such the node behind them has an extreme cost
            child->g = current->g + (currentDir * -1 + startPos == child.get()->gridPosition ? 100 : 1);
            //Manhattan distance
            child->h = abs(child->gridPosition.x - goal->gridPosition.x) + abs(child->gridPosition.y - goal->gridPosition.y);  // heuristic
            child->f = child->g + child->h;   // score

            // Is child in the open list?
            auto childInOpenList = [&]()
            {
                for (auto& existing : openList)
                {
                    if (child->gridPosition == existing->gridPosition)
                    {
                        // Child already in the open list. If this "version" has better score(less) than the existing, update it
                        if (child->f < existing->f)
                            existing = move(child);
                        return true;
                    }
                }
                return false;
            };
            if (childInOpenList()) continue;   // No need to check closed list or move child in open

            // Is child in the closed list?
            auto childInClosedList = [&]()
            {
                for (const auto& existing : closedList)
                {
                    // Child already in the closed list but with a greater score
                    if (child->gridPosition == existing->gridPosition && child->f > existing->f)
                        return true;
                    else if (child->gridPosition == existing->gridPosition && child->f < existing->f)
                    {
                        // Remove node from the closedList since a better path to it (less score)
                        // has been found. The node will be readded to the openList next.
                        closedList.remove(existing);
                        break;
                    }
                }
                return false;
            };
            // No need to move child to the open list if already in closed
            if (childInClosedList()) continue;
            openList.push_back(move(child));
        }
        std::sort(openList.begin(), openList.end(), [](const std::unique_ptr<Node>& a, const std::unique_ptr<Node>& b) { return a->f < b->f;});
        closedList.push_back(move(current));   // done with current, move to closedList
    }
    // No path to target, classic mode will take care of pathfinding
}