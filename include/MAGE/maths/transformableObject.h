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

#include "transformableBox.h"

namespace mage {

class MAGEDLL transformableObject : public transformableBox {
public:
	class MAGEDLL collisionBox {
	public:
		collisionBox();
		collisionBox(floatBox& fBox);

		floatBox getTransformed() const;
		floatBox getBase() const;

		void setBase(floatBox rect);

	public:
		hook<collisionBox*> onChanged;

	private:
		floatBox m_base;

		transformableObject* m_owner;
		friend class transformableObject;

	public:
		// serialization
		MAGE_DeclareSerializationList(
			m_base
		);
	};

public:
	// ctors (same as transformable)
	transformableObject();
	transformableObject(const pointF& pos, const pointF& size);
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

public:
	// serialization
	MAGE_DeclareSerializationList(
		MAGE_SerializedBase(transformableBox),
		MAGE_SerializedNVP("collisionBox", m_collisionBoxes)
	);
};

}