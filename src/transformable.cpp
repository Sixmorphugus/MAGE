#include "transformable.h"

using namespace mage;

mage::transformable::transformable()
{
}

mage::transformable::transformable(const pointF & position)
{
	m_position = position;
}

pointF mage::transformable::getPosition() const
{
	return m_position;
}

pointF mage::transformable::getAnchor() const
{
	return m_anchor;
}

pointF mage::transformable::getScale() const
{
	return m_scale;
}

float mage::transformable::getRotation() const
{
	return m_rotation;
}

void mage::transformable::setPosition(pointF & position)
{
	m_position = position;

	onTransformed.notify(this);
	onMoved.notify(this);
}

void mage::transformable::setAnchor(pointF & origin)
{
	m_anchor = origin;

	onTransformed.notify(this);
	onAnchored.notify(this);
}

void mage::transformable::setScale(pointF & sc)
{
	m_scale = sc;

	onTransformed.notify(this);
	onScaled.notify(this);
}

void mage::transformable::setRotation(float rot)
{
	m_rotation = rot;

	onTransformed.notify(this);
	onRotated.notify(this);
}

void mage::transformable::move(pointF & offset)
{
	auto cur = getPosition();
	setPosition(cur + offset);
}

void mage::transformable::shiftAnchor(pointF & offset)
{
	// not a default SFML thing
	auto curAnchor = getAnchor();
	setAnchor(curAnchor + offset);
}

void mage::transformable::rotate(float offset)
{
	auto cur = getRotation();
	setRotation(cur + offset);
}

void mage::transformable::scale(pointF & scalar)
{
	auto cur = getScale();
	setAnchor(cur + scalar);
}

pointF mage::transformable::getRealPosition()
{
	return getPosition() - sf::Vector2f(getAnchor().x * getScale().x, getAnchor().y * getScale().y);
}

void mage::transformable::setRealPosition(pointF p)
{
	setPosition(p + getAnchor());
}

void mage::transformable::pixelLock()
{
	setPosition(getPosition().floor());
	setAnchor(getAnchor().floor());

	setScale(pointF(1.f, 1.f));
	setRotation(0.f);
}

// SE
#include "scriptingEngine.h"

MAGE_DeclareScriptingType(transformable);
MAGE_DeclareScriptingConstructor(transformable(), "transformable");
MAGE_DeclareScriptingConstructor(transformable(const pointF& p), "transformable");
MAGE_DeclareScriptingConstructor(transformable(const transformable& p), "transformable");
MAGE_DeclareScriptingCopyOperator(transformable);
MAGE_DeclareScriptingFunction(&transformable::getAnchor, "getAnchor");
MAGE_DeclareScriptingFunction(&transformable::getPosition, "getPosition");
MAGE_DeclareScriptingFunction(&transformable::getRealPosition, "getRealPosition");
MAGE_DeclareScriptingFunction(&transformable::getRotation, "getRotation");
MAGE_DeclareScriptingFunction(&transformable::getScale, "getScale");
MAGE_DeclareScriptingFunction(&transformable::move, "move");
MAGE_DeclareScriptingFunction(&transformable::rotate, "rotate");
MAGE_DeclareScriptingFunction(&transformable::scale, "scale");
MAGE_DeclareScriptingFunction(&transformable::shiftAnchor, "shiftAnchor");
MAGE_DeclareScriptingFunction(&transformable::setAnchor, "setAnchor");
MAGE_DeclareScriptingFunction(&transformable::setRealPosition, "setRealPosition");
MAGE_DeclareScriptingFunction(&transformable::setPosition, "setPosition");
MAGE_DeclareScriptingFunction(&transformable::setRotation, "setRotation");
MAGE_DeclareScriptingFunction(&transformable::setScale, "setScale");
MAGE_DeclareScriptingFunction(&transformable::onAnchored, "onAnchored");
MAGE_DeclareScriptingFunction(&transformable::onMoved, "onMoved");
MAGE_DeclareScriptingFunction(&transformable::onRotated, "onRotated");
MAGE_DeclareScriptingFunction(&transformable::onScaled, "onScaled");
MAGE_DeclareScriptingFunction(&transformable::onTransformed, "onTransformed");

MAGE_DeclareScriptingHook("transformableHook", transformable*);