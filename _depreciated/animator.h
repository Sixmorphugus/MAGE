#pragma once

// mage::animator
// -------------
// Animation manager for a drawSprite.
// -------------
// Category: Rendering
// Has Hooks: Yes
// Script-Accessable: Yes
// -------------

#include "StdAfx.h"
#include "SfmlAfx.h"

#include "drawSprite.h"

namespace mage {

// basic rewrite of thor's animator class.
class drawSprite;

class MAGEDLL animator
{
public:
	struct animation
	{
		sf::Time time;
		std::vector<unsigned int> frames;
	};

public:
	animator(std::shared_ptr<resourceTexture> obj);

	void update(sf::Sprite &spr, sf::Time elapsed);

	bool isPlaying();
	std::string getPlaying();

	void add(std::string name, std::vector<unsigned int> frames, sf::Time duration);

	void play(std::string name, bool loop);
	void stop();

	bool isAnimation(std::string name);

	void setFrame(sf::Sprite &spr, unsigned int frame, bool interrupt = true);
	unsigned int getFrame(sf::Sprite &spr) const;

	unsigned int getFrameFromRect(sf::IntRect fr) const;

	unsigned int framesToEnd();

	void generateFrameRects(unsigned int spriteWidth, unsigned int spriteHeight);
	std::vector<unsigned int> getFrameList();

public:
	std::shared_ptr<resourceTexture> textureUsed;
	std::vector<sf::IntRect> frameRects;

private:
	void resetFrameEndTimer();

private:
	std::unordered_map<std::string, animation> animations;

	std::string curAnimationName;
	unsigned int curAnimationIteratorIndex;
	bool playing;
	bool autoStop;

	unsigned int animationProgress;
	sf::Time toFrameEnd;
};

} // namespace mage