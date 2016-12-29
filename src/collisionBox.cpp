#include "collisionBox.h"
#include "platform.h"

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
		point2f(m_owner->getPosition().x + (m_owner->getScale().x *m_base.position.x),
		m_owner->getPosition().y + (m_owner->getScale().y * m_base.position.y)),
		point2f(m_owner->getScale().x * m_base.size.x,
		m_owner->getScale().y * m_base.size.y)
	);
}

floatBox mage::collisionBox::getBase() const
{
	return m_base;
}

void mage::collisionBox::setBase(floatBox rect)
{
	m_base = rect;
	onChanged.notify(this);
}
