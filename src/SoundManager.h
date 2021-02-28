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

	const std::unique_ptr<sf::Texture> soundTexture;
	std::unique_ptr<sf::Sprite> soundSprite;
public:
	enum class Sound { Chomp, Eat_Fruit, Eat_Ghost, Extra_Life, Death, Intro };
	SoundManager(bool mute = false) 
		: muted(mute), soundTexture(Util::loadTexture(Config::sprites::sound)), soundSprite(Util::createSprite(*soundTexture, {Config::WIDTH, 0}))
	{
		soundSprite->setOrigin({ Config::sprites::size, 0.0f });
		soundSprite->setTextureRect({ 0, 0, Config::sprites::size, Config::sprites::size });
		for (int i = 0; i < soundBuffers.size(); ++i)
		{
			try {
				soundBuffers[i] = Util::loadSoundBuffer(Config::sounds::soundPaths[i]);
				sounds[i] = Util::createSound(*soundBuffers[i]);
			} catch (const std::exception&) { disabled = true; break; }
		}
	}

	void toggleSound()
	{
		muted = !muted;
		if (!disabled)
		{
			soundSprite->setTextureRect(muted ?
				sf::IntRect{ Config::sprites::size, 0, Config::sprites::size, Config::sprites::size } :
				sf::IntRect{ 0, 0, Config::sprites::size, Config::sprites::size });
		}	
	}

	void changeVolume(const float vol)
	{
		if (disabled) return;
		float newVolume = std::clamp(sounds[0]->getVolume() + vol, 0.0f, 100.0f);
		for (auto& sound : sounds) sound->setVolume(newVolume);
		if ((!muted && newVolume == 0.0f) || (muted && newVolume > 0.0f)) toggleSound();
	}

	bool isClicked(const sf::Vector2f& mouse) const { return soundSprite->getGlobalBounds().contains(mouse); }

	void play(const Sound s)
	{
		if (muted || disabled) return;
		if (sounds[static_cast<int>(s)]->getStatus() != sf::Sound::Playing) 
			sounds[static_cast<int>(s)]->play();
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const { if (!disabled) target.draw(*soundSprite); }
};