#pragma once
#include "SFML/Graphics.hpp"

#define NORTH sf::Vector2i{ 0, -1}
#define SOUTH sf::Vector2i{ 0,  1}
#define WEST  sf::Vector2i{-1,  0}
#define EAST  sf::Vector2i{ 1,  0}

enum class GhostState { Chase, Scatter, Frightened, Dead };
enum class TileType   { Dot, Wall, BigDot, Gate = 7, None = 8, Teleporter = 9 };

template <typename T>
auto distance(const T target, const T origin)
{
    T distanceVector = target - origin;
    return sqrt(distanceVector.x * distanceVector.x + distanceVector.y * distanceVector.y);
}

namespace Config 
{
  // Overall Scale of the game - Change ONLY this to adjust game size
	const float SCALE = 1.5f;  // No limit atm or dynamic (cmd also?)
  //-----------------------------------------------------------------
  //--------------------GAME WINDOW SIZE-----------------------------
  //-----------------------------------------------------------------
	const float WIDTH  = 448 * SCALE;
	const float HEIGHT = 496 * SCALE;
  //-----------------------------------------------------------------
  //--------------------GAME ENTITIES SIZES--------------------------
  //-----------------------------------------------------------------
	const float ENTITY_SIZE = 16.0f * SCALE;
	const float SCALED_OFFSET = ENTITY_SIZE / ( 3 * SCALE );
	const float DOT_SIZE    = 2.5f  * SCALE;
	const float B_DOT_SIZE  = 5.0f  * SCALE;
  //-----------------------------------------------------------------
	const int ROWS = 28, COLS = 31;  // change to std::array?

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
inline sf::Vector2f coordsToPosition(sf::Vector2i coords) { return { coords.x * Config::ENTITY_SIZE, coords.y * Config::ENTITY_SIZE }; }

// TODO:
//-----------------------------------------------------------------------------------------------------------------------------
// 6. Actual game logic:
//       Pacman states (alive - dead) - SOMEWHAT DONE
//       Inky getting in the tunnel..
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