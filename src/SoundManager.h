#pragma once
#include <array>
#include "SFML/Audio.hpp"
#include "Config.h"

class SoundManager : public sf::Drawable
{
private:
	bool muted = false;
	bool disabled = false;  // keeping this?
	const float volumePreset = 20.0f;
	const std::array<sf::SoundBuffer, 6> soundBuffers;  // do I need the buffers after setting the sounds? check docs
	std::array<sf::Sound, 6> sounds;

	sf::Texture soundTexture, noSoundTexture;
	sf::Sprite soundSprite;
public:
	enum class Sound { Chomp, Eat_Fruit, Eat_Ghost, Extra_Life, Death, Intro };
	SoundManager(bool mute = false) : muted(mute)
	{
		// load constant buffers
		// load buffers into sounds (should all take place in a lambda in cofig
		// use disabled after all?
		// changeVolume(volumePreset) - is this needed? what is the preset? check docs

		// load sound textures (mute/no mute) - in lambda like usual
		// load appr texture into sprite - based on muted
		// set sprite's pos - will be moved along with score to the top??
	}

	void toggle()
	{
		if (!disabled) muted ? soundSprite.setTexture(soundTexture) : soundSprite.setTexture(noSoundTexture);
		muted = !muted;
	}

	void changeVolume(float& v)
	{
		float newVolume = std::clamp(sounds[0].getVolume() + v, 0.0f, 100.0f);
		for (auto& sound : sounds) sound.setVolume(newVolume);
		if ((!muted && newVolume == 0.0f) || (muted && newVolume > 0.0f)) toggle();
	}

	bool isClicked(sf::Vector2f& mouse) const
	{
		sf::FloatRect bounds = soundSprite.getGlobalBounds();
		return bounds.contains(mouse);
	}

	void play(Sound s)
	{
		if (muted || disabled) return;
		sounds[static_cast<int>(s)].play();
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const { if (!disabled) target.draw(soundSprite); }
};