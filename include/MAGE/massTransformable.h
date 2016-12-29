#pragma once

// mage::massTransformable
// -------------
// A massTransformable is a transformable with a "mass" or "body":
// i.e. A transformable with collision boxes.
// It is used mainly as a base for game objects. It does not handle checking for the collisions because it does not have any way of accessing other known objects - 
// it simply provides some extra functions for making an object with a box composed body and managing said boxes.
// -------------
// Category: Maths
// Has Hooks: Yes
// Script-Accessable: Yes
// -------------

#include "collisionBox.h"

namespace mage {

class MAGEDLL massTransformable : public transformable {
public:
	// ctors (same as transformable)
	massTransformable();
	massTransformable(point2f pos, point2f size);

	// box management
	unsigned int addCollisionBox(std::shared_ptr<collisionBox> cBox);
	unsigned int getNumCollisionBoxes() const;
	void removeCollisionBox(unsigned int id);
	int indexOfCollisionBox(std::shared_ptr<collisionBox> cBox) const;
	std::shared_ptr<collisionBox> getCollisionBox(unsigned int id) const;

	// massTransformable
	point2f getBaseSize() const; // base size. 
	floatBox getBaseBox() const; // base box, generated from the position and size of the transformable
	floatBox getBoundingBox() const;

	void setBaseSize(point2f& newSize); // note that you should delete this function (like objSprited does) if you intend to control the base size yourself
	void incBaseSize(point2f& newSize);

public:
	hook<massTransformable*, unsigned int> onCollisionBoxAdded;
	hook<massTransformable*, unsigned int> onCollisionBoxRemoved;

	hook<massTransformable*> onResized;

private:
	std::vector<collisionBox> m_collisionBoxes;
	point2f m_size;
};

}