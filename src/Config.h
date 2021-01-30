#pragma once
#include "SFML/Graphics.hpp"

const sf::Vector2i NORTH{ 0, -1};
const sf::Vector2i SOUTH{ 0,  1};
const sf::Vector2i WEST {-1,  0};
const sf::Vector2i EAST { 1,  0};

enum class GhostState { Chase, Scatter, Frightened, Dead };
enum class TileType   { Dot, Wall, BigDot, Gate = 7, None = 8, Teleporter = 9 };

namespace Config 
{
  // Overall Scale of the game - Change ONLY this to adjust game size
    constexpr float SCALE = 1.5f;  // No limit atm or dynamic (cmd also?)
  //-----------------------------------------------------------------
  //--------------------GAME WINDOW SIZE-----------------------------
  //-----------------------------------------------------------------
    constexpr float WIDTH  = 448 * SCALE;
    constexpr float HEIGHT = 496 * SCALE;
  //-----------------------------------------------------------------
  //--------------------GAME ENTITIES SIZES--------------------------
  //-----------------------------------------------------------------
    constexpr float ENTITY_SIZE = 16.0f * SCALE;
    constexpr float SCALED_OFFSET = ENTITY_SIZE / ( 3 * SCALE );
    constexpr float DOT_SIZE    = 2.5f  * SCALE;
    constexpr float B_DOT_SIZE  = 5.0f  * SCALE;
  //-----------------------------------------------------------------
    constexpr const int ROWS = 28, COLS = 31;

    constexpr const char* grid = "resources/grid.txt";

    namespace Keybinds
    {
        constexpr sf::Keyboard::Key UP = sf::Keyboard::Key::W;
        constexpr sf::Keyboard::Key DOWN = sf::Keyboard::Key::S;
        constexpr sf::Keyboard::Key LEFT = sf::Keyboard::Key::A;
        constexpr sf::Keyboard::Key RIGHT = sf::Keyboard::Key::D;
        constexpr sf::Keyboard::Key ESC = sf::Keyboard::Key::Escape;
    }

    namespace sprites
    {
        constexpr const char* bckgnd = "resources/sprites/background.png";
        constexpr const char* pacman = "resources/sprites/pacman_spritesheet.png";
        constexpr const char* blinky = "resources/sprites/blinky_spritesheet.png";
        constexpr const char* pinky  = "resources/sprites/pinky_spritesheet.png";
        constexpr const char* inky   = "resources/sprites/inky_spritesheet.png";
        constexpr const char* clyde  = "resources/sprites/clyde_spritesheet.png";
        constexpr const char* hunted = "resources/sprites/ghost_spritesheet.png";
    }
}

namespace Util
{
    auto loadTexture = [](const char* path)
    {
        auto texture = std::make_unique<sf::Texture>();
        if (texture->loadFromFile(path)) return std::move(texture);
        throw std::exception("Cannot find texture");
    };

    template <typename T>
    auto distance(const T target, const T origin)
    {
        T distanceVector = target - origin;
        return sqrt(distanceVector.x * distanceVector.x + distanceVector.y * distanceVector.y);
    }

    inline sf::Vector2f coordsToPosition(sf::Vector2i coords) { return { coords.x * Config::ENTITY_SIZE, coords.y * Config::ENTITY_SIZE }; }
}

// TODO:
//-----------------------------------------------------------------------------------------------------------------------------
// 6. Actual game logic:
//       Pacman states (alive - dead) - SOMEWHAT DONE
//       Inky getting in the tunnel..
//       distance detection issues...
//       slow down pacman as well
//       some ghosts stay in the house until a number of dots have been eaten
//       Win / Loss
//       Multiple lives
//-----------------------------------------------------------------------------------------------------------------------------
// 7. Text - Media:
//       Score counter (class like snake?)
//       Lives counter
//       Game sound
//-----------------------------------------------------------------------------------------------------------------------------
// 8. Cleanup:
//       Various codebase improvements (consts, pointers, refs...)
//       Cmd arguments
//       Error handling for resource files and error messages
//       .... :)