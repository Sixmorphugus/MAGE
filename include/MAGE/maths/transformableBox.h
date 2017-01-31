#pragma once

// mage::transformableBox
// -------------
// A transformable object that has a size.
// -------------
// Category: Maths
// Has Hooks: Yes
// Script-Accessable: Yes
// -------------

#include "transformable.h"
#include "box.h"

namespace mage {

class MAGEDLL transformableBox : public transformable {
public:
	transformableBox();
	transformableBox(const pointF& pos);
	transformableBox(const pointF& pos, const pointF& size);

	// massTransformable
	pointF getCenter() const;

	pointF getBaseSize() const; // base size. 
	floatBox getBox() const; // base box, generated from the position and size of the transformable

	virtual void pixelLock();

protected:
	// by default the base size is 1 - the size of the object will be exactly what its scale is.
	// it can be changed by derived versions of the object.
	void setBaseSize(const pointF& newSize);
	void incBaseSize(const pointF& newSize);

	virtual void doTransformUpdate();

public:
	hook<transformableBox*> onResized;

private:
	pointF m_size;

public:
	// serialization
	MAGE_DeclareSerializationList(
		MAGE_SerializedBase(transformable),
		MAGE_SerializedNVP("size", m_size)
	);
};

}