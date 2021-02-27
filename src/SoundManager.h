#pragma once
#include <array>
#include "Config.h"

class SoundManager : public sf::Drawable
{
private:
	bool muted = false;
	bool disabled = false;
	/*const */std::array<std::unique_ptr<sf::SoundBuffer>, 6> soundBuffers;
	std::array<std::unique_ptr<sf::Sound>, 6> sounds;

	/*sf::Texture soundTexture, noSoundTexture;
	sf::Sprite soundSprite;*/
public:
	enum class Sound { Chomp, Eat_Fruit, Eat_Ghost, Extra_Life, Death, Intro };
	SoundManager(bool mute = false) : muted(mute)
	{
		for (int i = 0; i < soundBuffers.size(); ++i)
		{
			try {
				soundBuffers[i] = Util::loadSoundBuffer(Config::sounds::soundPaths[i]);
				sounds[i] = Util::createSound(*soundBuffers[i]);
			} catch (const std::exception&) { disabled = true; break; }
		}

		// load sound textures (mute/no mute) - in lambda like usual
		// load appr texture into sprite - based on muted
		// set sprite's pos - will be moved along with score to the top??
	}

	void toggleSound()
	{
		/*if (!disabled) muted ? soundSprite.setTexture(soundTexture) : soundSprite.setTexture(noSoundTexture);
		muted = !muted;*/
	}

	void changeVolume(const float vol)
	{
		float newVolume = std::clamp(sounds[0]->getVolume() + vol, 0.0f, 100.0f);
		for (auto& sound : sounds) sound->setVolume(newVolume);
		if ((!muted && newVolume == 0.0f) || (muted && newVolume > 0.0f)) toggleSound();
	}

	bool isClicked(const sf::Vector2f& mouse) const
	{
		/*sf::FloatRect bounds = soundSprite.getGlobalBounds();
		return bounds.contains(mouse);*/
	}

	void play(const Sound s)
	{
		if (muted || disabled) return;
		if (sounds[static_cast<int>(s)]->getStatus() != sf::Sound::Playing) 
			sounds[static_cast<int>(s)]->play();
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const { /*if (!disabled) target.draw(soundSprite);*/ }
};