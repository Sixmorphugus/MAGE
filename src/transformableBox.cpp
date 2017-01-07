#include "transformableBox.h"

using namespace mage;

transformableBox::transformableBox()
{
	m_size = pointF(1.f, 1.f);
}

transformableBox::transformableBox(pointF pos)
	: transformable(pos)
{
	m_size = pointF(1.f, 1.f);
}

transformableBox::transformableBox(pointF pos, pointF size)
	: transformable(pos)
{
	m_size = pointF(1.f, 1.f);
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
	onResized.notify(this);
}

void transformableBox::incBaseSize(const pointF& newSize)
{
	setBaseSize(getBaseSize() + newSize);
}

void transformableBox::pixelLock()
{
	m_size = m_size.floor();
}

// se
#include "scriptingEngine.h"

MAGE_DeclareScriptingType(transformableBox);
MAGE_DeclareScriptingBaseClass(transformable, transformableBox);
MAGE_DeclareScriptingConstructor(transformableBox(), "transformableObject");
MAGE_DeclareScriptingConstructor(transformableBox(const transformableBox&), "transformableObject");
MAGE_DeclareScriptingConstructor(transformableBox(pointF, pointF), "transformableObject");
MAGE_DeclareScriptingCopyOperator(transformableBox);
MAGE_DeclareScriptingFunction(&transformableBox::getBox, "getBox");