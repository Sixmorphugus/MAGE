#include "transformable.h"

using namespace mage;

point2f mage::transformable::getPosition() const
{
	return point2f(sf::Transformable::getPosition());
}

point2f mage::transformable::getAnchor() const
{
	return point2f(sf::Transformable::getOrigin());
}

point2f mage::transformable::getScale() const
{
	return point2f(sf::Transformable::getScale());
}

float mage::transformable::getRotation() const
{
	return sf::Transformable::getRotation();
}

void mage::transformable::setPosition(point2f & position)
{
	onTransformed.notify(this);
	onMoved.notify(this);

	sf::Transformable::setPosition(position.toSf());
}

void mage::transformable::setAnchor(point2f & origin)
{
	onTransformed.notify(this);
	onAnchored.notify(this);

	sf::Transformable::setOrigin(origin.toSf());
}

void mage::transformable::setScale(point2f & sc)
{
	onTransformed.notify(this);
	onScaled.notify(this);

	sf::Transformable::setScale(sc.toSf());
}

void mage::transformable::setRotation(float rot)
{
	onTransformed.notify(this);
	onRotated.notify(this);

	sf::Transformable::setRotation(rot);
}

void mage::transformable::move(point2f & offset)
{
	sf::Transformable::move(offset.toSf());

	onTransformed.notify(this);
	onMoved.notify(this);
}

void mage::transformable::shiftAnchor(point2f & offset)
{
	// not a default SFML thing
	auto curAnchor = getAnchor();
	setAnchor(curAnchor + offset);
}

void mage::transformable::rotate(float offset)
{
	sf::Transformable::rotate(offset);

	onTransformed.notify(this);
	onRotated.notify(this);
}

void mage::transformable::scale(point2f & scalar)
{
	sf::Transformable::scale(scalar.toSf());

	onTransformed.notify(this);
	onScaled.notify(this);
}

point2f mage::transformable::getRealPosition()
{
	return getPosition() - sf::Vector2f(getAnchor().x * getScale().x, getAnchor().y * getScale().y);
}

void mage::transformable::setRealPosition(point2f p)
{
	setPosition(p + getAnchor());
}

sf::Transform mage::transformable::getSfTransform()
{
	return getTransform();
}

sf::Transform mage::transformable::getSfInverseTransform()
{
	return getInverseTransform();
}

// SE
#include "scriptingEngine.h"

MAGE_DeclareScriptingType(transformable);
MAGE_DeclareScriptingConstructor(transformable(), "transformable");
MAGE_DeclareScriptingConstructor(transformable(const point2f& p), "transformable");
MAGE_DeclareScriptingConstructor(transformable(const transformable& p), "transformable");
MAGE_DeclareScriptingCopyOperator(transformable);
MAGE_DeclareScriptingFunction(&transformable::getAnchor, "getAnchor");
MAGE_DeclareScriptingFunction(&transformable::getPosition, "getPosition");
MAGE_DeclareScriptingFunction(&transformable::getRealPosition, "getRealPosition");
MAGE_DeclareScriptingFunction(&transformable::getRotation, "getRotation");
MAGE_DeclareScriptingFunction(&transformable::getScale, "getScale");
MAGE_DeclareScriptingFunction(&transformable::getSfInverseTransform, "getSfInverseTransform");
MAGE_DeclareScriptingFunction(&transformable::getSfTransform, "getSfTransform");
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