#pragma once

// mage::transformable
// -------------
// a version of the sf::Transformable object that doesn't require SFML's dll
// it includes hooks and will return mage::point2f rather than sf::Vector2f
// it wraps and expands upon the functionality of the SFML class.
// -------------
// Category: Maths
// Has Hooks: Yes
// Script-Accessable: Yes
// -------------

#include "point.h"
#include "hook.h"

namespace mage {

class MAGEDLL transformable {
public:
	transformable();
	transformable(const pointF& position);

	// SFML replacers
	pointF getPosition() const;
	pointF getAnchor() const;
	pointF getScale() const;
	float getRotation() const;
	float getDepth() const;

	pointF getRotationalCenter() const;

	void setPosition(const pointF& position);
	void setAnchor(const pointF& origin);
	void setScale(const pointF& sc);
	void setScale(float sc);
	void setRotation(float rot);

	void move(const pointF& offset);
	void shiftAnchor(const pointF& offset);
	void rotate(float rot);
	void scale(const pointF& scalar);
	void scale(float scalar);

	// additional functions
	pointF getRealPosition() const;
	void setRealPosition(const pointF p);

	virtual void pixelLock();
	
	std::vector<point2F> getTransformedPoints();

protected:
	virtual void doTransformUpdate();

public:
	// hooks
	hook<transformable*> onMoved;
	hook<transformable*> onRotated;
	hook<transformable*> onScaled;
	hook<transformable*> onAnchored;

	hook<transformable*> onTransformed;

protected:
	std::vector<point2F> m_transformedPoints;

private:
	float m_rotation; // objects in MAGE can only be rotated on one axis because we're still rendering sprites - 3D stuff simply isn't doable here

	pointF m_position, m_scale, m_anchor;

public:
	// serialization
	MAGE_DeclareSerializationList(
		MAGE_SerializedNVP("rotation", m_rotation),
		MAGE_SerializedNVP("position", m_position),
		MAGE_SerializedNVP("scale", m_scale),
		MAGE_SerializedNVP("anchor", m_anchor)
	);
};

}
