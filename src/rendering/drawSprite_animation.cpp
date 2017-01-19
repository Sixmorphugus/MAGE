#include "drawSprite.h"

using namespace mage;

drawSprite::animation::animation()
{
	fadeBetweenFrames = false;
	skipOnStop = true;
}

drawSprite::animation::animation(const interval& ft, const std::vector<unsigned int>& fl, bool fbf)
{
	frames = fl;
	fadeBetweenFrames = fbf;
	skipOnStop = true;
}

void drawSprite::animation::play(bool loop, unsigned int startFrame)
{
	onPlayed.notify(this);

	m_owner->m_autoStop = !loop;
	m_owner->m_animationProgress = startFrame;
	m_owner->m_curAnimation = findIndex();
	m_owner->m_toAnimationFrameEnd = frameTime;

	m_owner->onPlayAnimation.notify(m_owner);
}

bool drawSprite::animation::isPlaying() const
{
	return (m_owner->m_curAnimation == findIndex());
}

void drawSprite::animation::setParentFrameToAnimationFrame(unsigned int frame)
{
	m_owner->setFrame(frames[frame]);
}

unsigned int drawSprite::animation::findIndex() const
{
	for (unsigned int i = 0; i < m_owner->m_animations.size(); i++) {
		if (m_owner->m_animations[i].get() == this) {
			return i;
		}
	}

	return 0;
}

#include "scriptingEngine.h"

MAGE_DeclareScriptingType(drawSprite::animation);
MAGE_DeclareScriptingBaseClass(renamable, drawSprite::animation);
MAGE_DeclareScriptingConstructor(drawSprite::animation(), "drawSpriteAnimation");
MAGE_DeclareScriptingConstructor(drawSprite::animation(interval, std::vector<unsigned int>, bool), "drawSpriteAnimation");
MAGE_DeclareScriptingFunction(&drawSprite::animation::fadeBetweenFrames, "fadeBetweenFrames");
MAGE_DeclareScriptingFunction(&drawSprite::animation::findIndex, "findIndex");
MAGE_DeclareScriptingFunction(&drawSprite::animation::frames, "frames");
MAGE_DeclareScriptingFunction(&drawSprite::animation::frameTime, "frameTime");
MAGE_DeclareScriptingFunction(&drawSprite::animation::isPlaying, "isPlaying");
MAGE_DeclareScriptingFunction(&drawSprite::animation::skipOnStop, "skipOnStop");
MAGE_DeclareScriptingFunction(&drawSprite::animation::setParentFrameToAnimationFrame, "setParentFrameToAnimationFrame");
MAGE_DeclareScriptingFunction(&drawSprite::animation::onPlayed, "onPlayed");
MAGE_DeclareScriptingFunction(&drawSprite::animation::onStopped, "onStopped");