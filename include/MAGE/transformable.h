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

class MAGEDLL transformable : private sf::Transformable {
public:
	transformable();
	transformable(const point2f& position);

	// SFML replacers
	point2f getPosition() const;
	point2f getAnchor() const;
	point2f getScale() const;
	float getRotation() const;

	void setPosition(point2f& position);
	void setAnchor(point2f& origin);
	void setScale(point2f& origin);
	void setRotation(float rot);

	void move(point2f& offset);
	void shiftAnchor(point2f& offset);
	void rotate(float offset);
	void scale(point2f& scalar);

	// additional functions
	point2f getRealPosition();

	void setRealPosition(point2f p);

	// sfml converters
	sf::Transform getSfTransform();
	sf::Transform getSfInverseTransform();

public:
	// hooks
	hook<transformable*> onMoved;
	hook<transformable*> onRotated;
	hook<transformable*> onScaled;
	hook<transformable*> onAnchored;

	hook<transformable*> onTransformed;
};

}