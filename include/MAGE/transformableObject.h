#pragma once

// mage::transformableObject
// -------------
// A transformableObject is a transformable with a "mass" or "body":
// i.e. A transformable with collision boxes.
// It is used mainly as a base for game objects. It does not handle checking for the collisions because it does not have any way of accessing other known objects - 
// it simply provides some extra functions for making an object with a box composed body and managing said boxes in 3D space.
// -------------
// Category: Maths
// Has Hooks: Yes
// Script-Accessable: Yes
// -------------

#include "transformable.h"
#include "collisionBox.h"

namespace mage {

class MAGEDLL transformableBox : public virtual transformable {
public:
	transformableBox();
	transformableBox(pointF pos, pointF size);

	// massTransformable
	pointF getCenter() const;

	pointF getBaseSize() const; // base size. 
	floatBox getBaseBox() const; // base box, generated from the position and size of the transformable

	void setBaseSize(pointF& newSize); // note that you should delete this function and its sibling (like gmoSprited does) if you intend to control the base size yourself
	void incBaseSize(pointF& newSize);

	virtual void pixelLock();

public:
	hook<transformableBox*> onResized;

private:
	pointF m_size;
};

class MAGEDLL transformableObject : public virtual transformableBox {
public:
	// ctors (same as transformable)
	transformableObject();
	transformableObject(pointF pos, pointF size);
	transformableObject(const transformableObject& copy);

	transformableObject& operator=(const transformableObject& copy);

	// box management
	unsigned int addCollisionBox(std::shared_ptr<collisionBox>);
	unsigned int getNumCollisionBoxes() const;
	void removeCollisionBox(unsigned int id);
	std::shared_ptr<collisionBox> getCollisionBox(unsigned int id) const;

	virtual void pixelLock();

	floatBox getBoundingBox() const;

public:
	hook<transformableObject*, unsigned int> onCollisionBoxAdded;
	hook<transformableObject*, unsigned int> onCollisionBoxRemoved;

private:
	void copyTransformableObject(const transformableObject& from);

	std::vector<std::shared_ptr<collisionBox>> m_collisionBoxes;
};

}