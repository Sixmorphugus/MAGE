#pragma once

// mage::drawSprite
// -------------
// A version of drawBox that's geared more towards drawing textures.
// Is a renderable.
// Can (in theory) do everything that sf::Sprite can. As a result, so can drawRect (though this is much better in terms of usability)
// Also, animations.
// -------------
// Category: Rendering
// Has Hooks: Yes
// Script-Accessable: Yes
// -------------

#include "drawBox.h"

namespace mage {

class MAGEDLL drawSprite : public drawBox {
public:
	class MAGEDLL animation {
	public:
		animation(time& ft, std::vector<unsigned int>& frameList, bool fbf = false);

		void play(bool loop = true);
		void stop();

	public:
		hook<animation*> onPlayed;
		hook<animation*> onStopped;

		bool fadeBetweenFrames;
		time frameTime;

		std::vector<unsigned int> frames;

	private:
		drawSprite* m_owner;
		friend class drawSprite;
	};

public:
	drawSprite();
	drawSprite(std::shared_ptr<resourceTexture> tex);
	drawSprite(pointF& position, pointF& size, std::shared_ptr<resourceTexture> tex = nullptr);

	// texture management
	void setTexture(std::shared_ptr<resourceTexture>& tex);
	std::shared_ptr<resourceTexture> getTexture() const;

	floatBox getSpriteTextureCoords();
	void setSpriteTextureCoords(floatBox& box);

	void sizeToTexture(std::shared_ptr<resourceTexture> tex);
	void mapTextureCoordsToFullTexture(std::shared_ptr<resourceTexture> tex);

	// frames
	void regenerateFrames(point2U frameSize);
	void setFrame(unsigned int frame, bool interrupt = true); // if interrupt is true and an animation is playing it'll be stopped.
	unsigned int getFrame(sf::Sprite &spr) const;

	unsigned int getFrameFromRect(sf::IntRect fr) const;

	unsigned int framesToEnd();

	std::vector<unsigned int> getFrameList();

	// animation
	void continueAnimation(sf::Time elapsed);

	bool isPlayingAnAnimation();

	unsigned int getNumAnimations();
	std::shared_ptr<animation> getPlayingAnimation();
	std::shared_ptr<animation> getAnimation(unsigned int id);
	std::shared_ptr<animation> getAnimation(std::string name);

	unsigned int addAnimation(std::shared_ptr<animation> newAnim);

	void removeAnimation(unsigned int id);
	void removeAnimation(std::string name);

	void indexOfAnimation(std::shared_ptr<animation> anim);

public:
	hook<drawSprite*, animation*> onAddAnimation;
	hook<drawSprite*, animation*> onRemoveAnimation;

	hook<drawSprite*> onPlayAnimation;
	hook<drawSprite*> onStopAnimation;
	hook<drawSprite*> onFrameChanged;

private:
	void resetFrameEndTimer();

private:
	std::vector<std::shared_ptr<animation>> m_animations;

	std::vector<sf::IntRect> m_frameRectangles;

	unsigned int m_curAnimationId;
	bool m_playingAnimation;
	bool m_autoStopAnimation;

	unsigned int m_animationProgress;
	sf::Time m_toAnimationFrameEnd;

	friend class animation;
};

}