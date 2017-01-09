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

	// conversion to other point type
	template<typename To>
	point<To> convertAxis() const;

public:
	T x, y, z;
};

template<typename T> class point2 : private point<T> {
public:
	point2();
	point2(T x, T y);
	point2(T angle2D);
	point2(const sf::Vector2<T>& in);
	point2(std::string& in);

	// Arithmetic operators
	point2<T>& operator+=(const point2<T> &rhs);
	point2<T>& operator-=(const point2<T> &rhs);
	point2<T>& operator*=(const point2<T> &rhs);
	point2<T>& operator/=(const point2<T> &rhs);

	point2<T>& operator*=(const T rhs);
	point2<T>& operator/=(const T rhs);

	point2<T> operator+(const point2<T> &rhs);
	point2<T> operator-(const point2<T> &rhs);
	point2<T> operator*(const point2<T> &rhs);
	point2<T> operator/(const point2<T> &rhs);

	point2<T> operator*(const T rhs);
	point2<T> operator/(const T rhs);

	using point<T>::operator==;
	using point<T>::operator!=;

	point2<T> floor() const; // floor the two values of a vector.
	point2<T> ceil() const; // ceil the two values of a vector.
	using point<T>::getAngle2D;
	using point<T>::getLength;
	using point<T>::getDistanceTo;
	using point<T>::getAngle2DTo;
	using point<T>::getDot;
	using point<T>::getCross;

	using point<T>::toSf2;
	
	std::string toString() const;

	// conversion to other point type
	template<typename To>
	point2<To> convertAxis() const;

public:
	using point<T>::x;
	using point<T>::y;
};

typedef point<float> pointF;
typedef point<int> pointI;
typedef point<unsigned int> pointU;

typedef point2<float> point2F;
typedef point2<int> point2I;
typedef point2<unsigned int> point2U;

template<typename T>
template<typename To>
inline point<To> point<T>::convertAxis() const
{
	point<To> newPt;

	newPt.x = (To)x;
	newPt.y = (To)y;
	newPt.z = (To)z;
}

template<typename T>
template<typename To>
inline point2<To> point2<T>::convertAxis() const
{
	point2<To> newPt;

	newPt.x = (To)x;
	newPt.y = (To)y;
}
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
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::point<type>&, mage::point<type>, const mage::point<type>&>(&mage::point<type>::operator/=), "/=");\
\
MAGE_DeclareScriptingConstructor(mage::point2<type>(), name); \
MAGE_DeclareScriptingConstructor(mage::point2<type>(type), name); \
MAGE_DeclareScriptingConstructor(mage::point2<type>(type, type), name); \
MAGE_DeclareScriptingConstructor(mage::point2<type>(mage::point2<type>&), name); \
MAGE_DeclareScriptingConstructor(mage::point2<type>(std::string), name); \
MAGE_DeclareScriptingFunction(&mage::point2<type>::ceil, "ceil"); \
MAGE_DeclareScriptingFunction(&mage::point2<type>::floor, "floor"); \
MAGE_DeclareScriptingFunction(&mage::point2<type>::getAngle2DTo, "getAngle2DTo"); \
MAGE_DeclareScriptingFunction(&mage::point2<type>::getDistanceTo, "getDistanceTo"); \
MAGE_DeclareScriptingFunction(&mage::point2<type>::getDot, "getDot"); \
MAGE_DeclareScriptingFunction(&mage::point2<type>::getCross, "getCross"); \
MAGE_DeclareScriptingFunction(&mage::point2<type>::getLength, "getLength"); \
MAGE_DeclareScriptingFunction(&mage::point2<type>::getAngle2D, "getAngle2D"); \
MAGE_DeclareScriptingCopyOperator(mage::point2<type>); \
MAGE_DeclareScriptingFunction(&mage::point2<type>::operator+, "+"); \
MAGE_DeclareScriptingFunction(&mage::point2<type>::operator-, "-"); \
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::point2<type>, mage::point2<type>, const type>(&mage::point2<type>::operator*), "*"); \
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::point2<type>, mage::point2<type>, const type>(&mage::point2<type>::operator/), "/"); \
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::point2<type>, mage::point2<type>, const mage::point2<type>&>(&mage::point2<type>::operator*), "*"); \
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::point2<type>, mage::point2<type>, const mage::point2<type>&>(&mage::point2<type>::operator/), "/"); \
MAGE_DeclareScriptingFunction(&mage::point2<type>::operator+=, "+="); \
MAGE_DeclareScriptingFunction(&mage::point2<type>::operator-=, "-"); \
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::point2<type>&, mage::point2<type>, const type>(&mage::point2<type>::operator*=), "*="); \
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::point2<type>&, mage::point2<type>, const type>(&mage::point2<type>::operator/=), "/="); \
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::point2<type>&, mage::point2<type>, const mage::point2<type>&>(&mage::point2<type>::operator*=), "*="); \
MAGE_DeclareScriptingCustom(chaiscript::fun<mage::point2<type>&, mage::point2<type>, const mage::point2<type>&>(&mage::point2<type>::operator/=), "/=");

#define MAGE_DeclareScriptingpointType(type) MAGE_DeclareScriptingPointTypeNamed(type, "point" STRING(type))