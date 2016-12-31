#include "collisionBox.h"
#include "transformableObject.h"

using namespace mage;

collisionBox::collisionBox()
{
	m_owner = nullptr;
}

collisionBox::collisionBox(floatBox& fBox)
{
	m_owner = nullptr;
	m_base = fBox;
}

floatBox collisionBox::getTransformed() const
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

floatBox collisionBox::getBase() const
{
	return m_base;
}

void collisionBox::setBase(floatBox rect)
{
	m_base = rect;
	onChanged.notify(this);
}

#include "scriptingEngine.h"

MAGE_DeclareScriptingType(collisionBox);
MAGE_DeclareScriptingConstructor(collisionBox(), "collisionBox");
MAGE_DeclareScriptingConstructor(collisionBox(floatBox), "collisionBox");
MAGE_DeclareScriptingFunction(&collisionBox::getBase, "getBase");
MAGE_DeclareScriptingFunction(&collisionBox::getTransformed, "getTransformed");
MAGE_DeclareScriptingFunction(&collisionBox::onChanged, "onChanged");
MAGE_DeclareScriptingFunction(&collisionBox::setBase, "setBase");

MAGE_DeclareScriptingHook("collisionBoxHook", collisionBox*);