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

    void updateDisplay() { scoreText->setString(std::to_string(score)); }
public:
    ScoreDisplay() : font(Util::loadFont())
    {
        scoreText = std::make_unique<sf::Text>();
        scoreText->setFont(*font);
        scoreText->setCharacterSize(Config::textCharSize);
        scoreText->setFillColor(sf::Color::Green);
        scoreText->setPosition(Config::WIDTH / 2.0f, Config::HEIGHT - Config::BANNER_HEIGHT);
        updateDisplay();
    }

    void reset() { score = 0; updateDisplay(); }

    ScoreDisplay& operator++()
    {
        score += 10;    /*kinda meh*/
        updateDisplay();
        return *this;
    }

    int operator%(const int rhs) { return score % rhs; }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const { target.draw(*scoreText); }
};