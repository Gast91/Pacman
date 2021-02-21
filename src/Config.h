#pragma once
#include <map>
#include "SFML/Graphics.hpp"

const sf::Vector2i NORTH   { 0, -1};
const sf::Vector2i SOUTH   { 0,  1};
const sf::Vector2i WEST    {-1,  0};
const sf::Vector2i EAST    { 1,  0};
const sf::Vector2i INVALID { -INT_MAX, -INT_MAX };

enum class GhostState { Waiting, Chase, Scatter, Frightened, Dead, Paused };
enum class TileType   { Dot, Wall, BigDot, Gate = 7, None = 8, Teleporter = 9 };

namespace Config 
{
  // Overall Scale of the game - Change ONLY this to adjust game size
    constexpr float SCALE = 1.5f;  // No limit atm or dynamic (cmd also?)
  //-----------------------------------------------------------------
  //--------------------GAME WINDOW SIZE-----------------------------
  //-----------------------------------------------------------------
    constexpr unsigned int BANNER_HEIGHT = static_cast<unsigned int>(16 * SCALE);
    constexpr unsigned int WIDTH         = static_cast<unsigned int>(448 * SCALE);
    constexpr unsigned int HEIGHT        = static_cast<unsigned int>(496 * SCALE + BANNER_HEIGHT);
  //-----------------------------------------------------------------
  //--------------------GAME ENTITIES SIZES--------------------------
  //-----------------------------------------------------------------
    constexpr float ENTITY_SIZE        = 16.0f * SCALE;
    constexpr float SCALED_OFFSET      = ENTITY_SIZE / ( 3 * SCALE );
    constexpr float DOT_SIZE           = 2.5f  * SCALE;
    constexpr float B_DOT_SIZE         = 5.0f  * SCALE;
    constexpr float SPRITE_TEXT_OFFSET = 20.0f * SCALE;
  //-----------------------------------------------------------------
    constexpr int ROWS = 28, COLS = 31;

    constexpr const char* grid     = "resources/grid.txt";
    constexpr const char* fontFile = "resources/fonts/arial.ttf";

    constexpr unsigned int textCharSize = static_cast<unsigned int>(12 * SCALE);

    namespace Keybinds
    {
        constexpr sf::Keyboard::Key UP    = sf::Keyboard::Key::W;
        constexpr sf::Keyboard::Key DOWN  = sf::Keyboard::Key::S;
        constexpr sf::Keyboard::Key LEFT  = sf::Keyboard::Key::A;
        constexpr sf::Keyboard::Key RIGHT = sf::Keyboard::Key::D;
        constexpr sf::Keyboard::Key ESC   = sf::Keyboard::Key::Escape;
        constexpr sf::Keyboard::Key START = sf::Keyboard::Key::Space;
    }

    namespace sprites
    {
        // Individual Sprite Size
        constexpr int size = 16;
        constexpr const char* bckgnd   = "resources/sprites/background.png";
        constexpr const char* pacman   = "resources/sprites/pacman_spritesheet.png";
        constexpr const char* pacDeath = "resources/sprites/pacman_death_spritesheet.png";
        constexpr const char* blinky   = "resources/sprites/blinky_spritesheet.png";
        constexpr const char* pinky    = "resources/sprites/pinky_spritesheet.png";
        constexpr const char* inky     = "resources/sprites/inky_spritesheet.png";
        constexpr const char* clyde    = "resources/sprites/clyde_spritesheet.png";
        constexpr const char* hunted   = "resources/sprites/ghost_spritesheet.png";
        constexpr const char* lives    = "resources/sprites/lives_counter.png";
        constexpr const char* ghostP   = "resources/sprites/ghost_points.png";
    }

    const std::map <std::pair<int, int>, unsigned int> offsetDict = { {{ EAST.x,  EAST.y }, 0 }, {{ WEST.x,  WEST.y }, 2 },
                                                                      {{ NORTH.x, NORTH.y}, 4 }, {{ SOUTH.x, SOUTH.y}, 6 }};
}

namespace Util
{
    auto loadTexture = [](const char* path)
    {
        auto texture = std::make_unique<sf::Texture>();
        if (texture->loadFromFile(path)) return std::move(texture);
        throw std::exception("Cannot find texture");
    };

    auto loadFont = []()
    {
        auto font = std::make_unique<sf::Font>();
        if (font->loadFromFile(Config::fontFile)) 
            return std::move(font);
        throw std::exception("Cannot find font");
    };

    auto createSprite = [](const sf::Texture& texture, sf::Vector2f pos = { 0.0f, 0.0f })
    {
        std::unique_ptr<sf::Sprite> sprite;
        sprite = std::make_unique<sf::Sprite>(texture);
        sprite->scale(Config::SCALE, Config::SCALE);
        sprite->setPosition(pos);
        return std::move(sprite);
    };

    inline sf::Vector2i clampToGrid(const sf::Vector2i coords)
    {
        return { (coords.x < 1) ? 1 : (Config::ROWS - 1 < coords.x) ? Config::ROWS - 1 : coords.x,
                 (coords.y < 1) ? 1 : (Config::COLS - 1 < coords.y) ? Config::COLS - 1 : coords.y };
    }

    template <typename T>
    auto distance(const T target, const T origin)
    {
        T distanceVector = target - origin;
        return distanceVector.x * distanceVector.x + distanceVector.y * distanceVector.y;
    }

    inline sf::Vector2f coordsToPosition(sf::Vector2i coords) { return { coords.x * Config::ENTITY_SIZE, coords.y * Config::ENTITY_SIZE }; }
}

// TODO:
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Issues:
//     - sf::deltaV for movement - slow down pacman as well
//     - sf::clock for timers rather than mine
//     - collision improvements and general speed improvements - needed for new lvls also
//     - game 'pauses' for a sec when pacman eats a ghost and point text is shown (technically the dead ghost speed through to its home but whatevs)
//     - hunted anim 'flashes' only during the end of hunted period - otherwise it stays blue (do they swap back to hunting when they reach home or wait for hunted to end?)
//     - ghosts CAN use tunnels, but at decreased speed.. - teleporter neighbor?
//     - inky (and another) very rarely gets in the ghost house without being frightened
// Additions:
//     - pacman starts as a ball until 'space' is pressed (press space text is shown?)
//     - 70 dots and then 170 --> fruit (time limit?) - how often they appear in a level? banner displays the fruits at this stage (previous lvl fruits + current)
//     - cherries(100pts - lvl1) | Strawb(300pts - lvl2) | Orange(500pts - lvl3,4) | Apple(700pts - lvl5,6) | Grape(1kpts - lvl7,8) | Ship(2kpts - lvl10)
//       Bell(3kpts - lvl11,12) | Key(5kpts - lvl13+)
//     - Win / Loss / Ready! / Start etc Text - figure where to place it
//     - Game sound
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------