#pragma once

// mage::point<type>
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

#include "stringHelpers.h"

namespace mage {

template<typename T> class point {
public:
	point();
	point(T x, T y, T z = 0);
	point(T angle2D);
	point(const sf::Vector2<T>& in);
	point(const sf::Vector3<T>& in);
	point(std::string& in);

	// the implicit copy operators and operator= are fine.

	// Arithmetic operators
	point<T>& operator+=(const point<T> &rhs);
	point<T>& operator-=(const point<T> &rhs);
	point<T>& operator*=(const point<T> &rhs);
	point<T>& operator/=(const point<T> &rhs);

	point<T>& operator*=(const T rhs);
	point<T>& operator/=(const T rhs);

	point<T> operator+(const point<T> &rhs);
	point<T> operator-(const point<T> &rhs);
	point<T> operator*(const point<T> &rhs);
	point<T> operator/(const point<T> &rhs);

	point<T> operator*(const T rhs);
	point<T> operator/(const T rhs);

	// comparison operators
	bool operator== (const point<T> &rhs) const;
	bool operator!= (const point<T> &rhs) const;

	// some functions that used to live in maths.h
	point<T> floor() const; // floor the two values of a vector.
	point<T> ceil() const; // ceil the two values of a vector.
	T getAngle2D() const; // get the angles of the vector
	T getLength() const; // pythagoras.
	T getDistanceTo(point<T>& v2) const; // the distance between two positions as a float.
	T getAngle2DTo(point<T>& v2) const; // angle between two positions as a float.
	T getDot(point<T>& v2) const; // dot product.
	T getCross(point<T>& v2) const; // dot product.

	// conversion to the sfml types
	sf::Vector2<T> toSf2() const;
	sf::Vector3<T> toSf3() const;

	// conversion to string
	std::string toString() const;

public:
	T x, y, z;
};

typedef point<float> pointF;
typedef point<int> pointI;
typedef point<unsigned int> pointU;

}

// inline definitions
#include "point.inl"

// Scripting engine declaration macro
#define MAGE_DeclareScriptingPointTypeNamed(type, name) MAGE_DeclareScriptingType(mage::point<type>, name);\
MAGE_DeclareScriptingConstructor(mage::point<type>(), name);\
MAGE_DeclareScriptingConstructor(mage::point<type>(type), name);\
MAGE_DeclareScriptingConstructor(mage::point<type>(type, type), name);\
MAGE_DeclareScriptingConstructor(mage::point<type>(mage::point<type>&), name);\
MAGE_DeclareScriptingConstructor(mage::point<type>(std::string), name);\
MAGE_DeclareScriptingFunction(&mage::point<type>::ceil, "ceil");\
MAGE_DeclareScriptingFunction(&mage::point<type>::floor, "floor");\
MAGE_DeclareScriptingFunction(&mage::point<type>::getAngle2DTo, "getAngle2DTo");\
MAGE_DeclareScriptingFunction(&mage::point<type>::getDistanceTo, "getDistanceTo");\
MAGE_DeclareScriptingFunction(&mage::point<type>::getDot, "getDot");\
MAGE_DeclareScriptingFunction(&mage::point<type>::getCross, "getCross");\
MAGE_DeclareScriptingFunction(&mage::point<type>::getLength, "getLength");\
MAGE_DeclareScriptingFunction(&mage::point<type>::getAngle2D, "getAngle2D");\
MAGE_DeclareScriptingCopyOperator(mage::point<type>);\
MAGE_DeclareScriptingFunction(&mage::point<type>::operator+, "+");\
MAGE_DeclareScriptingFunction(&mage::point<type>::operator-, "-");\
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::point<type>, mage::point<type>, const type>(&mage::point<type>::operator*), "*");\
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::point<type>, mage::point<type>, const type>(&mage::point<type>::operator/), "/");\
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::point<type>, mage::point<type>, const mage::point<type>&>(&mage::point<type>::operator*), "*");\
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::point<type>, mage::point<type>, const mage::point<type>&>(&mage::point<type>::operator/), "/");\
MAGE_DeclareScriptingFunction(&mage::point<type>::operator+=, "+=");\
MAGE_DeclareScriptingFunction(&mage::point<type>::operator-=, "-");\
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::point<type>&, mage::point<type>, const type>(&mage::point<type>::operator*=), "*=");\
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::point<type>&, mage::point<type>, const type>(&mage::point<type>::operator/=), "/=");\
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::point<type>&, mage::point<type>, const mage::point<type>&>(&mage::point<type>::operator*=), "*=");\
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::point<type>&, mage::point<type>, const mage::point<type>&>(&mage::point<type>::operator/=), "/=");

#define MAGE_DeclareScriptingpointType(type) MAGE_DeclareScriptingPointTypeNamed(type, "point" STRING(type))