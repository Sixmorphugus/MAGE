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
#include "interval.h"
#include "namable.h"

namespace mage {

class MAGEDLL drawSprite : public drawBox {
public:
	class MAGEDLL animation : public renamable {
	public:
		animation();
		animation(const interval& ft, const std::vector<unsigned int>& frameList, bool fbf = false);

		void play(bool dontLoop = false, unsigned int startFrame = 0);
		bool isPlaying() const;

		void setParentFrameToAnimationFrame(unsigned int frame);

		unsigned int findIndex() const;

	public:
		hook<animation*> onPlayed;
		hook<animation*> onStopped;

		bool fadeBetweenFrames;
		bool skipOnStop;
		interval frameTime;

		std::vector<unsigned int> frames;

	private:
		drawSprite* m_owner;

		friend class drawSprite;
	};

public:
	drawSprite();
	drawSprite(std::shared_ptr<resourceTexture> tex);
	drawSprite(const pointF& position, std::shared_ptr<resourceTexture> tex = nullptr);

	// texture management
	void setTexture(std::shared_ptr<resourceTexture>& tex);
	std::shared_ptr<resourceTexture> getTexture() const;

	floatBox getSpriteTextureCoords();
	void setSpriteTextureCoords(const floatBox& box);

	// frames
	void regenerateFrames(const point2U& frameSize = point2U());
	void setFrame(unsigned int frame, bool interrupt = true); // if interrupt is true and an animation is playing it'll be stopped.
	int getFrameIndex(const floatBox& fr) const;

	floatBox getFrame(point2U position) const;
	floatBox getFrame(unsigned int index) const;
	unsigned int getNumFrames() const;

	// animation
	void continueAnimation(const interval& elapsed);

	void stopAnimation(); // stop playing and reset animation position to 0.

	bool isPlayingAnAnimation() const;

	unsigned int getNumAnimations();
	std::shared_ptr<animation> getCurrentAnimation() const;
	std::shared_ptr<animation> getAnimation(unsigned int id) const;
	std::shared_ptr<animation> getAnimation(std::string name) const;

	unsigned int addAnimation(std::shared_ptr<animation> newAnim);

	void removeAnimation(unsigned int id);
	void removeAnimation(std::string name);

	unsigned int indexOfAnimation(std::shared_ptr<animation> anim) const;

	unsigned int getAnimationProgress();

protected:
	virtual renderRecipe generateDrawRecipe(); // calls parent and overlays a fade animation on top if current animation has fade.

public:
	hook<drawSprite*, animation*> onAddAnimation;
	hook<drawSprite*, animation*> onRemoveAnimation;

	hook<drawSprite*> onPlayAnimation;
	hook<drawSprite*> onStopAnimation;

	hook<drawSprite*> onFrameChanged;

private:
	void resetFrameEndTimer();

	void mapTextureCoordsToFullTexture(std::shared_ptr<resourceTexture> tex);

private:
	std::vector<std::shared_ptr<animation>> m_animations;

	unsigned int m_animationProgress;
	interval m_toAnimationFrameEnd;
	bool m_autoStop;
	unsigned int m_rows; // number of rows assumed by the sprite atlas version of getFrame.

	int m_curAnimation;

	std::vector<floatBox> m_frameRectangles;

	friend class animation;
};

}