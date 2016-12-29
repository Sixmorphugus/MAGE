#pragma once

// mage::point2<type>
// -------------
// a templated point class and some maths functions for it.
// it greatly expands upon the functionality of the SFML class for game-oriented purposes.
// it's called point to differentiate it from the default std::vector type, which is just a dynamic array.
// -------------
// Category: Maths
// Has Hooks: No
// Script-Accessable: Yes
// -------------

#include "StdAfx.h"
#include "SfmlAfx.h"

namespace mage {

template<typename T> class point2 {
public:
	point2();
	point2(T x, T y);
	point2(T angle);
	point2(const sf::Vector2<T>& in);

	// the implicit copy operators and operator= are fine.

	// Arithmetic operators
	point2& operator+=(const point2<T> &rhs);
	point2& operator-=(const point2<T> &rhs);
	point2& operator*=(const point2<T> &rhs);
	point2& operator/=(const point2<T> &rhs);

	point2& operator*=(const T rhs);
	point2& operator/=(const T rhs);

	point2 operator+(const point2<T> &rhs);
	point2 operator-(const point2<T> &rhs);
	point2 operator*(const point2<T> &rhs);
	point2 operator/(const point2<T> &rhs);

	point2 operator*(const T rhs);
	point2 operator/(const T rhs);

	// comparison operators
	bool operator== (const point2<T> &rhs) const;
	bool operator!= (const point2<T> &rhs) const;

	// some functions that used to live in maths.h
	point2 floor() const; // floor the two values of a vector.
	point2 ceil() const; // ceil the two values of a vector.
	float getAngle() const; // get the direction a vector points.
	float getLength() const; // pythagoras.
	float getDistanceTo(point2<T>& v2) const; // the distance between two positions as a float.
	float getDirectionTo(point2<T>& v2) const; // angle between two positions as a float.
	float getDotProduct(point2<T>& v2) const; // dot product.

	// conversion to the sfml type
	sf::Vector2<T> toSf() const;

public:
	T x, y;
};

typedef point2<float> point2f;
typedef point2<int> point2i;
typedef point2<unsigned int> point2u;
}

// inline definitions
#include "point.inl"

// Scripting engine declaration macro
#define MAGE_DeclareScriptingPointTypeNamed(type, name) MAGE_DeclareScriptingType(mage::point2<type>, name);\
MAGE_DeclareScriptingConstructor(mage::point2<type>(), name);\
MAGE_DeclareScriptingConstructor(mage::point2<type>(type), name);\
MAGE_DeclareScriptingConstructor(mage::point2<type>(type, type), name);\
MAGE_DeclareScriptingConstructor(mage::point2<type>(mage::point2<type>&), name);\
MAGE_DeclareScriptingFunction(&mage::point2<type>::ceil, "ceil");\
MAGE_DeclareScriptingFunction(&mage::point2<type>::floor, "floor");\
MAGE_DeclareScriptingFunction(&mage::point2<type>::getDirectionTo, "getDirectionTo");\
MAGE_DeclareScriptingFunction(&mage::point2<type>::getDistanceTo, "getDistanceTo");\
MAGE_DeclareScriptingFunction(&mage::point2<type>::getDotProduct, "getDotProduct");\
MAGE_DeclareScriptingFunction(&mage::point2<type>::getLength, "getLength");\
MAGE_DeclareScriptingFunction(&mage::point2<type>::getAngle, "getAngle");\
MAGE_DeclareScriptingCopyOperator(mage::point2<type>);\
MAGE_DeclareScriptingFunction(&mage::point2<type>::operator+, "+");\
MAGE_DeclareScriptingFunction(&mage::point2<type>::operator-, "-");\
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::point2<type>, mage::point2<type>, const type>(&mage::point2<type>::operator*), "*");\
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::point2<type>, mage::point2<type>, const type>(&mage::point2<type>::operator/), "/");\
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::point2<type>, mage::point2<type>, const mage::point2<type>&>(&mage::point2<type>::operator*), "*");\
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::point2<type>, mage::point2<type>, const mage::point2<type>&>(&mage::point2<type>::operator/), "/");\
MAGE_DeclareScriptingFunction(&mage::point2<type>::operator+=, "+=");\
MAGE_DeclareScriptingFunction(&mage::point2<type>::operator-=, "-");\
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::point2<type>&, mage::point2<type>, const type>(&mage::point2<type>::operator*=), "*=");\
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::point2<type>&, mage::point2<type>, const type>(&mage::point2<type>::operator/=), "/=");\
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::point2<type>&, mage::point2<type>, const mage::point2<type>&>(&mage::point2<type>::operator*=), "*=");\
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::point2<type>&, mage::point2<type>, const mage::point2<type>&>(&mage::point2<type>::operator/=), "/=");

#define MAGE_DeclareScriptingPointType(type) MAGE_DeclareScriptingPointTypeNamed(type, "point2" STRING(type))
