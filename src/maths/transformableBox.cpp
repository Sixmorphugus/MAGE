#include "transformableBox.h"

using namespace mage;

transformableBox::transformableBox()
{
	m_size = pointF(1.f, 1.f);
}

transformableBox::transformableBox(const pointF& pos)
	: transformable(pos)
{
	m_size = pointF(1.f, 1.f, 1.f);
}

transformableBox::transformableBox(const pointF& pos, const pointF& size)
	: transformable(pos)
{
	m_size = pointF(1.f, 1.f, 1.f);
	setScale(size);
}

pointF transformableBox::getCenter() const
{
	return pointF(getPosition().x + (m_size.x / 2.f), getPosition().y + (m_size.y / 2.f));
}

pointF transformableBox::getBaseSize() const
{
	return m_size;
}

floatBox transformableBox::getBox() const
{
	return floatBox(getRealPosition(), getBaseSize() * getScale());
}

void transformableBox::setBaseSize(const pointF& newSize)
{
	m_size = newSize;

	doTransformUpdate();
	onResized.notify(this);
}

void transformableBox::incBaseSize(const pointF& newSize)
{
	setBaseSize(getBaseSize() + newSize);
}

void transformableBox::doTransformUpdate()
{
	m_transformedPoints.clear();

	// were this object to be drawn...
	// ...where would the verteces be?

	// This is harder than with point. We have 4 box coords to worry about now.
	point2F point1 = getBox().getLines()[0].start.to2();
	point2F point2 = getBox().getLines()[0].getEndPoint().to2();
	point2F point3 = getBox().getLines()[1].start.to2();
	point2F point4 = getBox().getLines()[1].getEndPoint().to2();

	// rotate all of these around the "origin"
	auto o = getRotationalCenter().to2();

	point1 = point1.getRotatedAround(o, getRotation());
	point2 = point2.getRotatedAround(o, getRotation());
	point3 = point3.getRotatedAround(o, getRotation());
	point4 = point4.getRotatedAround(o, getRotation());

	m_transformedPoints.push_back(point1);
	m_transformedPoints.push_back(point2);
	m_transformedPoints.push_back(point3);
	m_transformedPoints.push_back(point4);

	// call onTransformed hook.
	onTransformed.notify(this);
}

void transformableBox::pixelLock()
{
	m_size = m_size.getFloor();
}

// se
#include "scriptingEngine.h"

MAGE_DeclareScriptingType(transformableBox);
MAGE_DeclareScriptingBaseClass(transformable, transformableBox);
MAGE_DeclareScriptingConstructor(transformableBox(), "transformableBox");
MAGE_DeclareScriptingConstructor(transformableBox(const transformableBox&), "transformableBox");
MAGE_DeclareScriptingConstructor(transformableBox(pointF, pointF), "transformableBox");
MAGE_DeclareScriptingCopyOperator(transformableBox);
MAGE_DeclareScriptingFunction(&transformableBox::getBox, "getBox");

MAGE_DeclareSerializationBase(transformableBox);