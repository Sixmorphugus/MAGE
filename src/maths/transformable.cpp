#include "transformable.h"

using namespace mage;

transformable::transformable()
{
	m_rotation = 0.f;
	doTransformUpdate();
}

transformable::transformable(const pointF & position)
{
	m_rotation = 0.f;
	setPosition(position);
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

float mage::transformable::getDepth() const
{
	return m_position.z;
}

pointF transformable::getRotationalCenter() const
{
	return getAnchor() * getScale();
}

void transformable::setPosition(const pointF & position)
{
	m_position = position;

	doTransformUpdate();
	onMoved.notify(this);
}

void transformable::setAnchor(const pointF & origin)
{
	m_anchor = origin;

	doTransformUpdate();
	onAnchored.notify(this);
}

void transformable::setScale(const pointF & sc)
{
	m_scale = sc;

	doTransformUpdate();
	onScaled.notify(this);
}

void transformable::setScale(float sc)
{
	m_scale = pointF(sc, sc, sc);

	doTransformUpdate();
	onScaled.notify(this);
}

void transformable::setRotation(float rot)
{
	m_rotation = rot;

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
	setScale(cur + scalar);
}

void transformable::scale(float scalar)
{
	auto cur = getScale();
	setScale(cur + pointF(scalar, scalar, scalar));
}

pointF transformable::getRealPosition() const
{
	return getPosition() - (getRotationalCenter());
}

void transformable::setRealPosition(pointF p)
{
	setPosition(p + getAnchor());
}

void transformable::pixelLock()
{
	m_position = m_position.getFloor();
	m_anchor = m_anchor.getFloor();

	m_scale = pointF(1.f, 1.f);
	m_rotation = 0.f;
}

std::vector<point2F> transformable::getTransformedPoints()
{
	return m_transformedPoints;
}

void transformable::doTransformUpdate()
{
	m_transformedPoints.clear();

	// were this object to be drawn...
	// ...where would the verteces be?

	// for the base transform, we only need to know where one point is.
	// The position.
	point2F pos = getRealPosition().to2();

	if (getRotation() != 0) {
		// this is where it gets tricky - if the anchor has moved, we have to rotate around it.
		pos = pos.getRotatedAround(getRotationalCenter().to2(), getRotation());
	}

	// add our new point
	m_transformedPoints.push_back(pos);

	// notify hook
	onTransformed.notify(this);
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
MAGE_DeclareScriptingCustom(chaiscript::fun<void, transformable, const pointF&>(&transformable::scale), "scale");
MAGE_DeclareScriptingCustom(chaiscript::fun<void, transformable, float>(&transformable::scale), "scale");
MAGE_DeclareScriptingFunction(&transformable::shiftAnchor, "shiftAnchor");
MAGE_DeclareScriptingFunction(&transformable::setAnchor, "setAnchor");
MAGE_DeclareScriptingFunction(&transformable::setRealPosition, "setRealPosition");
MAGE_DeclareScriptingFunction(&transformable::setPosition, "setPosition");
MAGE_DeclareScriptingFunction(&transformable::setRotation, "setRotation");
MAGE_DeclareScriptingCustom(chaiscript::fun<void, transformable, const pointF&>(&transformable::setScale), "setScale");
MAGE_DeclareScriptingCustom(chaiscript::fun<void, transformable, float>(&transformable::setScale), "setScale");
MAGE_DeclareScriptingFunction(&transformable::onAnchored, "onAnchored");
MAGE_DeclareScriptingFunction(&transformable::onMoved, "onMoved");
MAGE_DeclareScriptingFunction(&transformable::onRotated, "onRotated");
MAGE_DeclareScriptingFunction(&transformable::onScaled, "onScaled");
MAGE_DeclareScriptingFunction(&transformable::onTransformed, "onTransformed");

MAGE_DeclareScriptingHook("transformableHook", transformable*);