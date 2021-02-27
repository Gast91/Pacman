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
    constexpr const char* startMsg = "Press 'Space' to start!";

    constexpr unsigned int textCharSize = static_cast<unsigned int>(12 * SCALE);

    constexpr int DOT_VALUE  = 10;
    constexpr int BDOT_VALUE = 50;
    constexpr int TOTAL_DOTS = 246;   // isn't it supposed to be 244?
    constexpr int BONUS_LIFE = 10000; // bonus life every #

    const sf::Vector2i FRUIT_COORDS = { 13, 17 };

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
        constexpr const char* collectP = "resources/sprites/collectible_points.png";
        constexpr const char* collect  = "resources/sprites/collectibles.png";
    }

    const std::map <std::pair<int, int>, unsigned int> offsetDict = { {{ EAST.x,  EAST.y }, 0 }, {{ WEST.x,  WEST.y }, 2 },
                                                                      {{ NORTH.x, NORTH.y}, 4 }, {{ SOUTH.x, SOUTH.y}, 6 }};

    const std::map<int, int> collectiblePoints = { {1, 100}, {2, 300}, {3, 500}, {4, 500}, {5, 700}, {6, 700}, {7, 1000},
                                                   {8, 1000}, {9, 2000}, {10, 2000}, {11, 3000}, {12, 3000}, {13, 5000} };
}

namespace Util
{
    auto loadTexture = [](const char* path)
    {
        auto texture = std::make_unique<sf::Texture>();
        if (texture->loadFromFile(path)) return std::move(texture);
        throw std::exception("Cannot find texture");
    };

    // Transparent texture loading - black color mask
    auto loadTextureTrans = [](const char* path)
    {
        sf::Image img;
        if (!img.loadFromFile(path)) throw std::exception("Cannot find texture");
        img.createMaskFromColor(sf::Color::Black);
        auto texture = std::make_unique<sf::Texture>();
        texture->loadFromImage(img);
        return std::move(texture);
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
//     - collision improvements and general speed improvements - needed for new lvls also
//     - technically the eaten ghost speeds through to its home instead of being paused like the rest but whatevs
//     - hunted anim 'flashes' only during the end of hunted period - otherwise it stays blue (do they swap back to hunting when they reach home or wait for hunted to end?)
//     - ghosts CAN use tunnels, but at decreased speed.. - teleporter neighbor?
//     - inky (and another) very rarely gets in the ghost house without being frightened
// Additions:
//     - 70 dots and then 170 --> fruit (time limit?) - how often they appear in a level?
//     - cherries(100pts - lvl1) | Strawb(300pts - lvl2) | Orange(500pts - lvl3,4) | Apple(700pts - lvl5,6) | Grape(1kpts - lvl7,8) | Ship(2kpts - lvl10)
//       Bell(3kpts - lvl11,12) | Key(5kpts - lvl13+) - score size is 16 for up to 700pts, size 20 for rest..
//     - Game sound
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------