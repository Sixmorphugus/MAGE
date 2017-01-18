#include "transformableObject.h"

using namespace mage;

transformableObject::collisionBox::collisionBox()
{
	m_owner = nullptr;
}

transformableObject::collisionBox::collisionBox(floatBox& fBox)
{
	m_owner = nullptr;
	m_base = fBox;
}

floatBox transformableObject::collisionBox::getTransformed() const
{
	if (!m_owner) {
		//p::warn("Collision box missing owner - returning base box");
		return m_base;
	}

	return floatBox(
		pointF(m_owner->getPosition().x + (m_owner->getScale().x *m_base.position.x),
		m_owner->getPosition().y + (m_owner->getScale().y * m_base.position.y)),
		pointF(m_owner->getScale().x * m_base.size.x,
		m_owner->getScale().y * m_base.size.y)
	);
}

floatBox transformableObject::collisionBox::getBase() const
{
	return m_base;
}

void transformableObject::collisionBox::setBase(floatBox rect)
{
	m_base = rect;
	onChanged.notify(this);
}

#include "scriptingEngine.h"

MAGE_DeclareScriptingType(transformableObject::collisionBox);
MAGE_DeclareScriptingConstructor(transformableObject::collisionBox(), "collisionBox");
MAGE_DeclareScriptingConstructor(transformableObject::collisionBox(floatBox), "collisionBox");
MAGE_DeclareScriptingFunction(&transformableObject::collisionBox::getBase, "getBase");
MAGE_DeclareScriptingFunction(&transformableObject::collisionBox::getTransformed, "getTransformed");
MAGE_DeclareScriptingFunction(&transformableObject::collisionBox::onChanged, "onChanged");
MAGE_DeclareScriptingFunction(&transformableObject::collisionBox::setBase, "setBase");

MAGE_DeclareScriptingHook("collisionBoxHook", transformableObject::collisionBox*);