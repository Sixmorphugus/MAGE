#include "transformableObject.h"

using namespace mage;

transformableBox::transformableBox()
{
}

transformableBox::transformableBox(pointF pos, pointF size)
	: transformable(pos)
{
}

pointF transformableBox::getCenter() const
{
	return pointF(getPosition().x + (m_size.x / 2.f), getPosition().y + (m_size.y / 2.f));
}

pointF transformableBox::getBaseSize() const
{
	return m_size;
}

floatBox transformableBox::getBaseBox() const
{
	return floatBox(getRealPosition(), getBaseSize() * getScale());
}

void transformableBox::setBaseSize(pointF & newSize)
{
	m_size = newSize;
	onResized.notify(this);
}

void transformableBox::incBaseSize(pointF & newSize)
{
	setBaseSize(getBaseSize() + newSize);
}

void transformableBox::pixelLock()
{
	m_size = m_size.floor();
}


transformableObject::transformableObject()
{
}

transformableObject::transformableObject(pointF pos, pointF size)
{
	setPosition(pos);
	setBaseSize(size);
}

transformableObject::transformableObject(const transformableObject & copy)
{
	copyTransformableObject(copy);
}

transformableObject & transformableObject::operator=(const transformableObject & copy)
{
	copyTransformableObject(copy);
	return *this;
}

unsigned int transformableObject::addCollisionBox(std::shared_ptr<collisionBox> b)
{
	auto thisBox = getNumCollisionBoxes();

	b->m_owner = this;
	m_collisionBoxes.push_back(b);

	onCollisionBoxAdded.notify(this, thisBox);

	return thisBox;
}

unsigned int transformableObject::getNumCollisionBoxes() const
{
	return m_collisionBoxes.size();
}

void transformableObject::removeCollisionBox(unsigned int id)
{
	m_collisionBoxes.erase(m_collisionBoxes.begin() + id);
	onCollisionBoxRemoved.notify(this, id);
}

std::shared_ptr<collisionBox> transformableObject::getCollisionBox(unsigned int id) const
{
	return m_collisionBoxes[id];
}

void mage::transformableObject::pixelLock()
{
	for (unsigned int i = 0; i < m_collisionBoxes.size(); i++) {
		m_collisionBoxes[i]->setBase(m_collisionBoxes[i]->getBase().floor());
	}
}

floatBox transformableObject::getBoundingBox() const
{
	// gather collision boxes
	std::vector<floatBox> boxes;

	boxes.push_back(getBaseBox());
	
	for (unsigned int i = 0; i < m_collisionBoxes.size(); i++) {
		boxes.push_back(m_collisionBoxes[i]->getTransformed());
	}

	return floatBox(boxes);
}

void mage::transformableObject::copyTransformableObject(const transformableObject & to)
{
	m_collisionBoxes.clear();
	for (unsigned int i = 0; i < to.m_collisionBoxes.size(); i++) {
		m_collisionBoxes.push_back(std::make_shared<collisionBox>(m_collisionBoxes[i])); // make a copy
		m_collisionBoxes[i]->m_owner = this; // rather than the previous owner, we own the copy.
	}
}

#include "scriptingEngine.h"

MAGE_DeclareScriptingType(transformableObject);
MAGE_DeclareScriptingBaseClass(transformable, transformableObject);
MAGE_DeclareScriptingConstructor(transformableObject(), "transformableObject");
MAGE_DeclareScriptingConstructor(transformableObject(const transformableObject&), "transformableObject");
MAGE_DeclareScriptingConstructor(transformableObject(pointF, pointF), "transformableObject");
MAGE_DeclareScriptingCopyOperator(transformableObject);
MAGE_DeclareScriptingFunction(&transformableObject::addCollisionBox, "addCollisionBox");
MAGE_DeclareScriptingFunction(&transformableObject::getNumCollisionBoxes, "getNumCollisionBoxes");
MAGE_DeclareScriptingFunction(&transformableObject::removeCollisionBox, "removeCollisionBox");
MAGE_DeclareScriptingFunction(&transformableObject::getCollisionBox, "getCollisionBox");
MAGE_DeclareScriptingFunction(&transformableObject::getBaseSize, "getBaseSize");
MAGE_DeclareScriptingFunction(&transformableObject::getBaseBox, "getBaseBox");
MAGE_DeclareScriptingFunction(&transformableObject::getBoundingBox, "getBoundingBox");
MAGE_DeclareScriptingFunction(&transformableObject::setBaseSize, "setBaseSize");
MAGE_DeclareScriptingFunction(&transformableObject::incBaseSize, "incBaseSize");

MAGE_DeclareScriptingHook("transformableObjectCollisionHook", transformableObject*, unsigned int);
MAGE_DeclareScriptingHook("transformableObjectHook", transformableObject*);
