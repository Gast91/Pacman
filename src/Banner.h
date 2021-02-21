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

    void restart() {}   /*TODO for restarting the whole game*/
    int getLives() const { return lives; }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const { target.draw(*livesSprite); }
};

class ScoreDisplay : public sf::Drawable
{
private:
    unsigned int score = 0;
    const std::unique_ptr<sf::Font> font;
    std::unique_ptr<sf::Text> scoreText;

    const std::unique_ptr<sf::Texture> ghostPointsTexture;
    std::unique_ptr<sf::Sprite> ghostPoints;
    int ghostPointsCounter = 200;
    bool mutable showingGhostPoints = false;

    void updateDisplay() { scoreText->setString(std::to_string(score)); }
public:
    ScoreDisplay() : font(Util::loadFont()), ghostPointsTexture(Util::loadTexture(Config::sprites::ghostP)), ghostPoints(Util::createSprite(*ghostPointsTexture))
    {
        scoreText = std::make_unique<sf::Text>();
        scoreText->setFont(*font);
        scoreText->setCharacterSize(Config::textCharSize);
        scoreText->setFillColor(sf::Color::Green);
        scoreText->setPosition(Config::WIDTH / 2.0f, Config::HEIGHT - Config::BANNER_HEIGHT);
        updateDisplay();
    }

    void reset() { score = 0; updateDisplay(); }

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

    void resetGhostPoints() { ghostPointsCounter = 200; }

    ScoreDisplay& operator+=(const int rhs)
    {
        score += rhs;
        updateDisplay();
        return *this;
    }

    int operator%(const int rhs) { return score % rhs; }

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
    std::unique_ptr<sf::Sprite> collectSprite;
public:
    Collectible() 
        : collectTexture(Util::loadTexture(Config::sprites::collect)), 
          collectSprite(Util::createSprite(*collectTexture, { Config::WIDTH, Config::HEIGHT - Config::BANNER_HEIGHT }))
    {
        // This sprite is drawn from top right since it 'grows' leftward
        collectSprite->setTextureRect({ (12 - level) * Config::sprites::size, 0, Config::sprites::size * level, Config::sprites::size });
        collectSprite->setOrigin({ static_cast<float>(Config::sprites::size * level), 0.0f });
    }

    // spawn collectible?
    // collectible also has a score value

    Collectible& operator++()
    {
        if (level >= 12) return *this;  // from level 12 onwards the collectible is the key (last/leftmost sprite)
        ++level;
        // This sprite is drawn from top right since it 'grows' leftward
        collectSprite->setTextureRect({ (12 - level) * Config::sprites::size, 0, Config::sprites::size * level, Config::sprites::size });
        collectSprite->setOrigin({ static_cast<float>(Config::sprites::size * level), 0.0f });
        return *this;
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const { target.draw(*collectSprite); }
};