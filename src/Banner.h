#pragma once
#include "Config.h"

class Level;

class Lives : public sf::Drawable
{
private:
    friend Level;
	const std::unique_ptr<sf::Texture> livesText;
	const std::unique_ptr<sf::Sprite> livesSprite;
    int lives;
public:
    Lives(int totalLives = 2) 
        : livesText(Util::loadTexture(Config::sprites::lives)), 
          lives(totalLives), 
          livesSprite(Util::createSprite(*livesText, {0, Config::HEIGHT - Config::BANNER_HEIGHT }))
    {
        livesSprite->setTextureRect({ 0, 0, Config::sprites::size * lives, Config::sprites::size });
    }

    Lives& operator--()
    {
        --lives;
        livesSprite->setTextureRect({ 0, 0, Config::sprites::size * lives, Config::sprites::size });
        return *this;
    }
    void reset() {}
    int getLives() const { return lives; }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const { target.draw(*livesSprite); }
};