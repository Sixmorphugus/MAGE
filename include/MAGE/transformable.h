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
#include "hookableLifetimeObject.h"

namespace mage {

class MAGEDLL transformable : public hookableLifetimeObject {
public:
	transformable();
	transformable(const pointF& position);

	// SFML replacers
	pointF getPosition() const;
	pointF getAnchor() const;
	pointF getScale() const;
	float getRotation() const;

	void setPosition(pointF& position);
	void setAnchor(pointF& origin);
	void setScale(pointF& origin);
	void setRotation(float rot);

	void move(pointF& offset);
	void shiftAnchor(pointF& offset);
	void rotate(float rot);
	void scale(pointF& scalar);

	// additional functions
	pointF getRealPosition();
	void setRealPosition(pointF p);

	void pixelLock();

public:
	// hooks
	hook<transformable*> onMoved;
	hook<transformable*> onRotated;
	hook<transformable*> onScaled;
	hook<transformable*> onAnchored;

	hook<transformable*> onTransformed;

private:
	float m_rotation; // objects in MAGE can only be rotated on one axis because we're still rendering sprites - 3D stuff simply isn't doable here
	pointF m_position, m_anchor, m_scale;
};

}