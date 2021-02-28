#pragma once
#include <map>
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

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
    constexpr unsigned int BOT_BANNER_H  = static_cast<unsigned int>(16 * SCALE);
    constexpr unsigned int TOP_BANNER_H  = static_cast<unsigned int>(16 * SCALE);
    constexpr unsigned int WIDTH         = static_cast<unsigned int>(448 * SCALE);
    constexpr unsigned int HEIGHT        = static_cast<unsigned int>(496 * SCALE + BOT_BANNER_H + TOP_BANNER_H);
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
        constexpr sf::Keyboard::Key VOL_U = sf::Keyboard::Key::Add;
        constexpr sf::Keyboard::Key VOL_D = sf::Keyboard::Key::Subtract;
        constexpr sf::Keyboard::Key MUTE  = sf::Keyboard::Key::M;
        constexpr sf::Mouse::Button L_BUT = sf::Mouse::Button::Left;
    }

    namespace sprites
    {
        // Individual Sprite Size
        constexpr int size = 16;
        // Collectible_points2 sprite size
        constexpr int collectSize = 20;
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
        constexpr const char* sound    = "resources/sprites/sound.png";
    }

    namespace sounds
    {
        constexpr float volumePreset = 20.0f;
        constexpr const char* soundPaths[6] = {
            "resources/sounds/pacman_chomp.wav",
            "resources/sounds/pacman_eat_fruit.wav",
            "resources/sounds/pacman_eat_ghost.wav",
            "resources/sounds/pacman_extra_life.wav",
            "resources/sounds/pacman_death.wav",
            "resources/sounds/pacman_intro.wav" };
    }

    const std::map <std::pair<int, int>, unsigned int> offsetDict = { {{ EAST.x,  EAST.y }, 0 }, {{ WEST.x,  WEST.y }, 2 },
                                                                      {{ NORTH.x, NORTH.y}, 4 }, {{ SOUTH.x, SOUTH.y}, 6 }};

    const std::map<int, std::pair<int, sf::IntRect>> collectiblePoints = 
    { {1,  {100,  {Config::sprites::size * 0, 0,                     Config::sprites::size,        Config::sprites::size}}},
      {2,  {300,  {Config::sprites::size * 1, 0,                     Config::sprites::size,        Config::sprites::size}}},
      {3,  {500,  {Config::sprites::size * 2, 0,                     Config::sprites::size,        Config::sprites::size}}},
      {4,  {500,  {Config::sprites::size * 2, 0,                     Config::sprites::size,        Config::sprites::size}}},
      {5,  {700,  {Config::sprites::size * 3, 0,                     Config::sprites::size,        Config::sprites::size}}},
      {6,  {700,  {Config::sprites::size * 3, 0,                     Config::sprites::size,        Config::sprites::size}}},
      {7,  {1000, {Config::sprites::size * 0, Config::sprites::size, Config::sprites::collectSize, Config::sprites::size}}},
      {8,  {1000, {Config::sprites::size * 0, Config::sprites::size, Config::sprites::collectSize, Config::sprites::size}}},
      {9,  {2000, {Config::sprites::size * 1, Config::sprites::size, Config::sprites::collectSize, Config::sprites::size}}},
      {10, {2000, {Config::sprites::size * 1, Config::sprites::size, Config::sprites::collectSize, Config::sprites::size}}},
      {11, {3000, {Config::sprites::size * 2, Config::sprites::size, Config::sprites::collectSize, Config::sprites::size}}},
      {12, {3000, {Config::sprites::size * 2, Config::sprites::size, Config::sprites::collectSize, Config::sprites::size}}},
      {13, {5000, {Config::sprites::size * 3, Config::sprites::size, Config::sprites::collectSize, Config::sprites::size}}}};
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

    auto loadSoundBuffer = [](const char* path)
    {
        auto buffer = std::make_unique<sf::SoundBuffer>();
        if (buffer->loadFromFile(path)) return std::move(buffer);
        throw std::exception("Cannot find sound");
    };

    auto createSound = [](const sf::SoundBuffer& buffer, const float volPreset = Config::sounds::volumePreset)
    {
        std::unique_ptr<sf::Sound> sound;
        sound = std::make_unique<sf::Sound>(buffer);
        sound->setVolume(volPreset);
        return std::move(sound);
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

    inline sf::Vector2f coordsToPosition(sf::Vector2i coords) { return { coords.x * Config::ENTITY_SIZE, coords.y * Config::ENTITY_SIZE + Config::TOP_BANNER_H }; }
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
//     - pause(), resume() and the Timers need work for correct detection of which was running before pause
//     - if game ends, or next level while in hunted mode, ghosts textures are messed up before starting to move
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------