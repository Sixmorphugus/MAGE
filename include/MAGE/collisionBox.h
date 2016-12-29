#pragma once

// mage::collisionBox
// -------------
// can return its own position transformed in relation to an owning transformable.
// Mainly used by massTransformable.
// -------------
// Category: Maths
// Has Hooks: Yes
// Script-Accessable: Yes
// -------------

#include "transformable.h"
#include "box.h"

namespace mage {
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
	transformable* m_owner;
	friend class transformable;
};

}