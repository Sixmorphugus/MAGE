#pragma once

// mage::box<type>
// -------------
// a version of the sf::Rect object that doesn't require SFML's dll
// it greatly expands upon the functionality of the SFML class for game-oriented purposes.
// -------------
// Category: Maths
// Has Hooks: No
// Script-Accessable: Yes
// -------------

#include "point.h"
#include "maths.h"

namespace mage {

// BOX
// Mimics SFML's sf::Rect class but adds important functionality
enum boxInit {
	POSANDSIZE,
	CORNERS
};

template<typename T>
class box {
public:
	// ctors
	box();
	box(sf::Rect<T> conv);
	box(point2<T> corner1OrPosition, point2<T> corner2OrSize, boxInit inm = POSANDSIZE);
	box(std::vector<box<T>> fBoxes); // VERY COOL CONSTRUCTOR. Makes a box around all the other boxes.

	// functions for manipulating the rect
	void setCorner1(point2<T> p); // move the "top" corner without changing the other.
	void setCorner2(point2<T> p); // move the "bottom" corner without changing the other.

	void moveCorner1(point2<T> m);
	void moveCorner2(point2<T> m);

	point2<T> getSecondCorner() const;

	void scale(point2<T> scalar);

	// detection
	bool contains(point2<T>& pointIn) const;
	bool contains(box<T>& boxIn) const;
	bool intersects(box<T>& boxIn) const;

	rect<T> normalize() const; // if the rectangle has negative size, fix that.

	point2<T> clampedPoint(point2<T>& pointIn);

	point2<T> distanceToOuterEdge(point2<T>& pointIn);

	sf::Rect<T> toSf() const;

	// operators
	box<T>& operator*=(T rH);
	box<T>& operator/=(T rH);
	box<T>& operator*=(point2<T>& rH);
	box<T>& operator/=(point2<T>& rH);

	box<T> operator*(T rH);
	box<T> operator/(T rH);
	box<T> operator*(point2<T>& rH);
	box<T> operator/(point2<T>& rH);

public:
	point2<T> position, size;
};

typedef box<float> floatBox;
typedef box<int> intBox;
}

#include "box.inl"

// SCRIPTING macro
#define MAGE_DeclareScriptingBoxTypeNamed(type, name) MAGE_DeclareScriptingType(mage::box<type>, name);\
MAGE_DeclareScriptingConstructor(mage::box<type>(), name);\
MAGE_DeclareScriptingConstructor(mage::box<type>(sf::Rect<type> sfRect), name);\
MAGE_DeclareScriptingConstructor(mage::box<type>(point2<type>, point2<type>, boxInit), name);\
MAGE_DeclareScriptingCustom(chaiscript::fun([](point2<type> pos, point2<type> size) { return mage::box<type>(pos, size); }, name );\
MAGE_DeclareScriptingFunction(&mage::box<type>::setCorner1, "setCorner1");\
MAGE_DeclareScriptingFunction(&mage::box<type>::setCorner2, "setCorner2");\
MAGE_DeclareScriptingFunction(&mage::box<type>::moveCorner1, "moveCorner1");\
MAGE_DeclareScriptingFunction(&mage::box<type>::moveCorner2, "moveCorner2");\
MAGE_DeclareScriptingFunction(&mage::box<type>::getSecondCorner, "getSecondCorner");\
MAGE_DeclareScriptingFunction(&mage::box<type>::scale, "scale");\
MAGE_DeclareScriptingFunction(&mage::box<type>::intersects, "intersects");\
MAGE_DeclareScriptingFunction(&mage::box<type>::normalize, "normalize");\
MAGE_DeclareScriptingFunction(&mage::box<type>::clampedPoint, "clampedPoint");\
MAGE_DeclareScriptingFunction(&mage::box<type>::distanceToOuterEdge, "distanceToOuterEdge");\
MAGE_DeclareScriptingFunction(&mage::box<type>::toSf, "toSf");\
MAGE_DeclareScriptingCustom(chaiscript::fun<bool, mage::box<type>, point2<type>&>(&mage::box<type>::contains), "contains");\
MAGE_DeclareScriptingCustom(chaiscript::fun<bool, mage::box<type>, box<type>&>(&mage::box<type>::contains), "contains");\
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::box<type>, mage::box<type>, const type>(&mage::box<type>::operator*), "*");\
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::box<type>, mage::box<type>, const type>(&mage::box<type>::operator/), "/");\
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::box<type>, mage::box<type>, const mage::point2<type>&>(&mage::box<type>::operator*), "*");\
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::box<type>, mage::box<type>, const mage::point2<type>&>(&mage::box<type>::operator/), "/");\
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::box<type>&, mage::box<type>, const type>(&mage::box<type>::operator*=), "*="); \
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::box<type>&, mage::box<type>, const type>(&mage::box<type>::operator/=), "/="); \
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::box<type>&, mage::box<type>, const mage::point2<type>&>(&mage::box<type>::operator*=), "*="); \
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::box<type>&, mage::box<type>, const mage::point2<type>&>(&mage::box<type>::operator/=), "/=");

#define MAGE_DeclareScriptingBoxType(type) MAGE_DeclareScriptingPointTypeNamed(type, STRING(type) "Box")