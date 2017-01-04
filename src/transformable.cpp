#include "transformable.h"

using namespace mage;

transformable::transformable()
{
}

transformable::transformable(const pointF & position)
{
	m_position = position;
}

pointF transformable::getPosition() const
{
	return m_position;
}

pointF transformable::getAnchor() const
{
	return m_anchor;
}

pointF transformable::getScale() const
{
	return m_scale;
}

float transformable::getRotation() const
{
	return m_rotation;
}

void transformable::setPosition(const pointF & position)
{
	m_position = position;

	onTransformed.notify(this);
	onMoved.notify(this);
}

void transformable::setAnchor(const pointF & origin)
{
	m_anchor = origin;

	onTransformed.notify(this);
	onAnchored.notify(this);
}

void transformable::setScale(const pointF & sc)
{
	m_scale = sc;

	onTransformed.notify(this);
	onScaled.notify(this);
}

void transformable::setRotation(float rot)
{
	m_rotation = rot;

	onTransformed.notify(this);
	onRotated.notify(this);
}

void transformable::move(const pointF & offset)
{
	auto cur = getPosition();
	setPosition(cur + offset);
}

void transformable::shiftAnchor(const pointF & offset)
{
	// not a default SFML thing
	auto curAnchor = getAnchor();
	setAnchor(curAnchor + offset);
}

void transformable::rotate(float offset)
{
	auto cur = getRotation();
	setRotation(cur + offset);
}

void transformable::scale(const pointF & scalar)
{
	auto cur = getScale();
	setAnchor(cur + scalar);
}

pointF transformable::getRealPosition() const
{
	return getPosition() - sf::Vector2f(getAnchor().x * getScale().x, getAnchor().y * getScale().y);
}

void transformable::setRealPosition(pointF p)
{
	setPosition(p + getAnchor());
}

void transformable::pixelLock()
{
	m_position = m_position.floor();
	m_anchor = m_anchor.floor();

	m_scale = pointF(1.f, 1.f);
	m_rotation = 0.f;
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