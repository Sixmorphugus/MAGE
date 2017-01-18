#include "drawSprite.h"
#include "resourceTexture.h"

using namespace mage;

drawSprite::drawSprite()
{
}

drawSprite::drawSprite(std::shared_ptr<resourceTexture> tex)
{
	setTexture(tex);
}

drawSprite::drawSprite(pointF & position, pointF& size, std::shared_ptr<resourceTexture> tex)
	: drawBox(position, size)
{
	setTexture(tex);
}

void drawSprite::setTexture(std::shared_ptr<resourceTexture>& tex)
{
	states.texture = tex;

	sizeToTexture(tex);
	mapTextureCoordsToFullTexture(tex);

	regenerateFrames();
}

std::shared_ptr<resourceTexture> drawSprite::getTexture() const
{
	return states.texture.lock();
}

floatBox drawSprite::getSpriteTextureCoords()
{
	return floatBox(pointF(texCoord1.x, texCoord1.y), pointF(texCoord4.x, texCoord4.y), mage::CORNERS);
}

void drawSprite::setSpriteTextureCoords(floatBox& box)
{
	texCoord1 = point2F(box.position.x, box.position.y);
	texCoord2 = point2F(box.position.x + box.size.x, box.position.y);
	texCoord3 = point2F(box.position.x, box.position.y + box.size.y);
	texCoord4 = point2F(box.position.x + box.size.x, box.position.y + box.size.y);
}

void drawSprite::sizeToTexture(std::shared_ptr<resourceTexture> tex)
{
	if (!tex)
		return;

	setScale(tex->getSize().convertAxis<float>().to3());
}

void drawSprite::mapTextureCoordsToFullTexture(std::shared_ptr<resourceTexture> tex)
{
	if (!tex)
		return;

	setSpriteTextureCoords(floatBox(pointF(0.f, 0.f), tex->getSize().convertAxis<float>().to3()));
}

void drawSprite::regenerateFrames(point2U frameSize)
{
	m_frameRectangles.clear();

	auto tex = states.texture.lock();

	if (!tex)
		return;

	// how big is the texture?
	point2U texSize = tex->getSize();

	if (frameSize.x == 0 || frameSize.y == 0) {
		// if either frameSize coord is 0 a frame is made with the whole texture and it's left at that.
		m_frameRectangles.push_back(floatBox(pointF(0.f, 0.f), texSize.convertAxis<float>().to3()));
		m_rows = 1;
	}
	else {
		point2U atlasSize = texSize / frameSize;

		for (unsigned int x = 0; x < atlasSize.x; x++) {
			for (unsigned int y = 0; y < atlasSize.y; y++) {
				m_frameRectangles.push_back(floatBox(pointF((float)(frameSize.x * x), (float)(frameSize.y * y)), frameSize.convertAxis<float>().to3()));
			}
		}

		m_rows = atlasSize.y;
	}
}

void drawSprite::setFrame(unsigned int frame, bool interrupt)
{
	if (interrupt)
		stopAnimation();

	setSpriteTextureCoords(m_frameRectangles[frame]);

	onFrameChanged.notify(this);
}

int drawSprite::getFrameIndex(floatBox fr) const
{
	for (unsigned int i = 0; i < m_frameRectangles.size(); i++) {
		auto rect = m_frameRectangles[i];

		if (rect == fr) {
			return i;
		}
	}

	return -1;
}

floatBox drawSprite::getFrame(point2U position) const
{
	int index = position.y + (m_rows * position.x);
	return getFrame(index);
}

floatBox drawSprite::getFrame(unsigned int index) const
{
	if (index >= m_frameRectangles.size()) {
		return floatBox();
	}

	return m_frameRectangles[index];
}

unsigned int mage::drawSprite::getNumFrames() const
{
	return m_frameRectangles.size();
}

void drawSprite::resetFrameEndTimer()
{
	m_toAnimationFrameEnd = interval(0.f);
}

void drawSprite::continueAnimation(interval elapsed)
{
	if (!isPlayingAnAnimation())
		return;

	m_toAnimationFrameEnd -= elapsed;

	if (m_toAnimationFrameEnd.get() <= 0.f) {
		m_animationProgress++; // move to next animation frame

		// are we over the animation bounds?
		if (m_animationProgress >= getCurrentAnimation()->frames.size()) {
			m_animationProgress = 0;

			if (m_autoStop) {
				stopAnimation();
			}
		}

		// advance to next frame
		getCurrentAnimation()->setParentFrameToAnimationFrame(m_animationProgress);
	}
}

void drawSprite::stopAnimation()
{
	if (auto a = getCurrentAnimation()) {
		a->onStopped.notify(a.get());
	}

	m_curAnimation = -1;
	m_animationProgress = 0;

	onStopAnimation.notify(this);
}

bool drawSprite::isPlayingAnAnimation() const
{
	return (m_curAnimation != -1);
}

unsigned int drawSprite::getNumAnimations()
{
	return m_animations.size();
}

std::shared_ptr<drawSprite::animation> drawSprite::getCurrentAnimation() const
{
	if (m_curAnimation < 0) {
		return nullptr;
	}

	return getAnimation((unsigned int)m_curAnimation);
}

std::shared_ptr<drawSprite::animation> drawSprite::getAnimation(unsigned int id) const
{
	if (id >= m_animations.size())
		return nullptr;

	return m_animations[id];
}

std::shared_ptr<drawSprite::animation> drawSprite::getAnimation(std::string name) const
{
	for (unsigned int i = 0; i < m_animations.size(); i++) {
		if (m_animations[i]->getName() == name) {
			return m_animations[i];
		}
	}

	return nullptr;
}

unsigned int drawSprite::addAnimation(std::shared_ptr<animation> newAnim)
{
	newAnim->m_owner = this;
	m_animations.push_back(newAnim);

	onAddAnimation.notify(this, newAnim.get());
}

void drawSprite::removeAnimation(unsigned int id)
{
	if (id >= m_animations.size())
		return;

	onRemoveAnimation.notify(this, m_animations[id].get());

	m_animations[id]->m_owner = nullptr;
	m_animations.erase(m_animations.begin() + id);
}

void drawSprite::removeAnimation(std::string name)
{
	for (int i = 0; i < (int)m_animations.size(); i++) {
		if (m_animations[i]->getName() == name) {
			onRemoveAnimation.notify(this, m_animations[i].get());

			m_animations[i]->m_owner = nullptr;
			m_animations.erase(m_animations.begin() + i);

			i--;
		}
	}
}

unsigned int drawSprite::indexOfAnimation(std::shared_ptr<animation> anim) const
{
	return anim->findIndex();
}

unsigned int drawSprite::getAnimationProgress()
{
	return m_animationProgress;
}

renderRecipe drawSprite::generateDrawRecipe()
{
	auto recipe = drawBox::generateDrawRecipe();

	if (isPlayingAnAnimation()) {

	}

	return recipe;
}


#include "scriptingEngine.h"

MAGE_DeclareScriptingType(drawSprite);
MAGE_DeclareScriptingBaseClass(drawBox, drawSprite);
MAGE_DeclareScriptingBaseClass(transformableBox, drawSprite);
MAGE_DeclareScriptingBaseClass(transformable, drawSprite);
MAGE_DeclareScriptingConstructor(drawSprite(), "drawSprite");
MAGE_DeclareScriptingConstructor(drawSprite(pointF& position, pointF& size, std::shared_ptr<resourceTexture> tex), "drawSprite");
MAGE_DeclareScriptingConstructor(drawSprite(std::shared_ptr<resourceTexture> tex), "drawSprite");
MAGE_DeclareScriptingCopyOperator(drawSprite);
MAGE_DeclareScriptingFunction(&drawSprite::getSpriteTextureCoords, "getSpriteTextureCoords");
MAGE_DeclareScriptingFunction(&drawSprite::setSpriteTextureCoords, "setSpriteTextureCoords");
MAGE_DeclareScriptingFunction(&drawSprite::sizeToTexture, "sizeToTexture");
MAGE_DeclareScriptingFunction(&drawSprite::mapTextureCoordsToFullTexture, "mapTextureCoordsToFullTexture");
MAGE_DeclareScriptingFunction(&drawSprite::addAnimation, "addAnimation");
MAGE_DeclareScriptingFunction(&drawSprite::continueAnimation, "continueAnimation");
MAGE_DeclareScriptingFunction(&drawSprite::getAnimationProgress, "getAnimationProgress");
MAGE_DeclareScriptingFunction(&drawSprite::getCurrentAnimation, "getCurrentAnimation");
MAGE_DeclareScriptingFunction(&drawSprite::getFrameIndex, "getFrameIndex");
MAGE_DeclareScriptingFunction(&drawSprite::getNumAnimations, "getNumAnimations");
MAGE_DeclareScriptingFunction(&drawSprite::getNumFrames, "getNumFrames");
MAGE_DeclareScriptingFunction(&drawSprite::indexOfAnimation, "indexOfAnimation");
MAGE_DeclareScriptingFunction(&drawSprite::isPlayingAnAnimation, "isPlayingAnAnimation");
MAGE_DeclareScriptingFunction(&drawSprite::onAddAnimation, "onAddAnimation");
MAGE_DeclareScriptingFunction(&drawSprite::onRemoveAnimation, "onRemoveAnimation");
MAGE_DeclareScriptingFunction(&drawSprite::onPlayAnimation, "onPlayAnimation");
MAGE_DeclareScriptingFunction(&drawSprite::onStopAnimation, "onStopAnimation");
MAGE_DeclareScriptingCustom(chaiscript::fun<floatBox, drawSprite, unsigned int>(&drawSprite::getFrame), "getFrame");
MAGE_DeclareScriptingCustom(chaiscript::fun<floatBox, drawSprite, point2U>(&drawSprite::getFrame), "getFrame");
MAGE_DeclareScriptingCustom(chaiscript::fun<std::shared_ptr<drawSprite::animation>, drawSprite, unsigned int>(&drawSprite::getAnimation), "getAnimation");
MAGE_DeclareScriptingCustom(chaiscript::fun<std::shared_ptr<drawSprite::animation>, drawSprite, std::string>(&drawSprite::getAnimation), "getAnimation");
MAGE_DeclareScriptingCustom(chaiscript::fun<void, drawSprite, unsigned int>(&drawSprite::removeAnimation), "removeAnimation");
MAGE_DeclareScriptingCustom(chaiscript::fun<void, drawSprite, std::string>(&drawSprite::removeAnimation), "removeAnimation");