#pragma once
#include "Config.h"

class Level;

class Lives : public sf::Drawable
{
private:
    friend Level;
    const std::unique_ptr<sf::Texture> livesTexture; // kind of a waste since sprite already exists in pacMovement
    const std::unique_ptr<sf::Sprite> livesSprite;
    int lives;
public:
    Lives(int totalLives = 2) 
        : livesTexture(Util::loadTexture(Config::sprites::lives)),
          lives(totalLives), 
          livesSprite(Util::createSprite(*livesTexture, {0, Config::HEIGHT - Config::BANNER_HEIGHT }))
    {
        livesTexture->setRepeated(true);
        livesSprite->setTextureRect({ 0, 0, Config::sprites::size * lives, Config::sprites::size });
    }

    Lives& operator++()
    {
        ++lives;
        livesSprite->setTextureRect({ 0, 0, Config::sprites::size * lives, Config::sprites::size });
        return *this;
    }

    Lives& operator--()
    {
        --lives;
        livesSprite->setTextureRect({ 0, 0, Config::sprites::size * lives, Config::sprites::size });
        return *this;
    }

    void reset()
    {
        lives = 2;
        livesSprite->setTextureRect({ 0, 0, Config::sprites::size * lives, Config::sprites::size });
    }

    int getLives() const { return lives; }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const { target.draw(*livesSprite); }
};

class ScoreDisplay : public sf::Drawable
{
private:
    int score = 0;
    int threshold = Config::BONUS_LIFE;
    const std::unique_ptr<sf::Font> font;
    std::unique_ptr<sf::Text> scoreText;

    const std::unique_ptr<sf::Texture> ghostPointsTexture;
    std::unique_ptr<sf::Sprite> ghostPoints;
    int ghostPointsCounter = 200;
    bool started = false;
    bool mutable showingGhostPoints = false;

    /*bool mutable showingFruitPoints = false;
    const std::unique_ptr<sf::Texture> fruitPointsTexture;
    std::unique_ptr<sf::Sprite> fruitPoints;*/

    void updateDisplay() 
    {
        scoreText->setString(started ? std::to_string(score) : Config::startMsg);
        const float startXPos = static_cast<float>(Config::WIDTH) / 2.0f - scoreText->getLocalBounds().width / 2.0f;
        scoreText->setPosition(startXPos, Config::HEIGHT - Config::BANNER_HEIGHT);
    }
public:
    ScoreDisplay() : font(Util::loadFont()), ghostPointsTexture(Util::loadTextureTrans(Config::sprites::ghostP)), ghostPoints(Util::createSprite(*ghostPointsTexture))
    {
        scoreText = std::make_unique<sf::Text>();
        scoreText->setFont(*font);
        scoreText->setCharacterSize(Config::textCharSize);
        scoreText->setFillColor(sf::Color::Green);
        updateDisplay();
    }

    void reset()   { started = false; updateDisplay(); }
    void restart() { score = 0; reset(); }
    void start()   { started = true; updateDisplay(); }

    void displayGhostPoints(const sf::Vector2f& position)
    {
        int textureX = (ghostPointsCounter / 400) * Config::sprites::size;
        if (textureX == 64) textureX -= Config::sprites::size;

        ghostPoints->setTextureRect({ textureX, 0, Config::sprites::size, Config::sprites::size });
        ghostPoints->setPosition(position.x - Config::SPRITE_TEXT_OFFSET, position.y - Config::SPRITE_TEXT_OFFSET);
        score += ghostPointsCounter;
        showingGhostPoints = true;
        ghostPointsCounter *= 2;

        updateDisplay();
    }

    /*enum Points {GHOST_POINTS, FRUIT_POINTS};
    void displayPoints(const Points pointType, const sf::Vector2f& displayPos)
    {
        if (pointType == GHOST_POINTS)
    }*/

    bool overThreshold() 
    { 
        if (score >= threshold)
        {
            threshold += Config::BONUS_LIFE;
            return true;
        }
        return false;
    }

    void resetGhostPoints() { ghostPointsCounter = 200; }

    ScoreDisplay& operator+=(const int rhs)
    {
        score += rhs;
        updateDisplay();
        return *this;
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(*scoreText);
        if (showingGhostPoints)
        {
            target.draw(*ghostPoints);
            showingGhostPoints = false;
        }
    }
};

class Collectible : public sf::Drawable
{
private:
    int level = 1;
    const std::unique_ptr<sf::Texture> collectTexture;
    // This sprite is drawn from top right since it 'grows' leftward
    std::unique_ptr<sf::Sprite> collectSprite;

    // Fruit that gets spawn into the world
    std::unique_ptr<sf::Sprite> fruitSprite;
    mutable bool fruitVisible = false;
    mutable Timer fruitVisibleTimer;
public:
    Collectible() 
        : collectTexture(Util::loadTextureTrans(Config::sprites::collect)), 
          collectSprite(Util::createSprite(*collectTexture, { Config::WIDTH, Config::HEIGHT - Config::BANNER_HEIGHT })),
          fruitSprite(Util::createSprite(*collectTexture, Util::coordsToPosition(Config::FRUIT_COORDS)))
    {
        collectSprite->setTextureRect({ (12 - level) * Config::sprites::size, 0, Config::sprites::size * level, Config::sprites::size });
        collectSprite->setOrigin({ static_cast<float>(Config::sprites::size * level), 0.0f });

        fruitSprite->setTextureRect({ (12 - level) * Config::sprites::size, 0, Config::sprites::size, Config::sprites::size });
    }

    bool isVisible() const { return fruitVisible; }

    void spawnCollectible()
    {
        fruitVisible = true;
        fruitVisibleTimer.startTimer();
    }

    int eat()
    {
        fruitVisible = false;
        fruitVisibleTimer.stopTimer();
        return Config::collectiblePoints.at(level);
    }

    Collectible& operator++()
    {
        if (level >= 12) return *this;  // from level 12 onwards the collectible is the key (last/leftmost sprite)
        ++level;
        collectSprite->setTextureRect({ (12 - level) * Config::sprites::size, 0, Config::sprites::size * level, Config::sprites::size });
        fruitSprite->setTextureRect({ (12 - level) * Config::sprites::size, 0, Config::sprites::size, Config::sprites::size });
        collectSprite->setOrigin({ static_cast<float>(Config::sprites::size * level), 0.0f });
        return *this;
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const 
    { 
        target.draw(*collectSprite);
        if (fruitVisible && fruitVisibleTimer.isRunning() && fruitVisibleTimer.msEllapsed() / 1000 < 10) target.draw(*fruitSprite);
        else if (fruitVisibleTimer.isRunning() && fruitVisibleTimer.msEllapsed() / 1000 >= 10)
        {
            fruitVisible = false;
            fruitVisibleTimer.stopTimer();
        }
    }
};