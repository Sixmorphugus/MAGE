#pragma once

// mage::line
// -------------
// A line between two points.
// Can perform similar operations to a raycast.
// Not an SFML class.
// -------------
// Category: Maths
// Has Hooks: No
// Script-Accessable: Yes
// -------------

#include "point.h"
#include "maths.h"

namespace mage {

	template<typename T>
	class line {
	public:
		line();
		line(const point<T>& start, const point2<T>& endOrSize, shapeInit initMode = POSANDSIZE);
		line(const point2<T>& zeroVector);
		line(T zeroAngle);

		int orientation(point2<T>& r) const;

		bool intersects(const point<T>& point) const;
		bool intersects(const line<T>& point) const;

		point<T> getPointDifference() const;
		float getDistance() const;
		float getAngle() const;

		void setStartOffset(point<T>& offset);
		void setEndPoint(point<T>& offset);
		point<T> getEndPoint() const;

	public:
		point<T> start;
		point2<T> endOffset;
	};

	typedef line<float> lineF;
	typedef line<int> lineI;
	typedef line<unsigned int> lineU;
}

#include "line.inl"

#define MAGE_DeclareScriptingLine(T, name)\
MAGE_DeclareScriptingTypeNamed(mage::line<T>, name);\
MAGE_DeclareScriptingConstructor(mage::line<T>(), name);\
MAGE_DeclareScriptingConstructor(mage::line<T>(const mage::line<T>&), name);\
MAGE_DeclareScriptingConstructor(mage::line<T>(const mage::point<T>& p1, const mage::point2<T>& p2), name);\
MAGE_DeclareScriptingConstructor(mage::line<T>(const mage::point2<T>& zeroPoint), name);\
MAGE_DeclareScriptingConstructor(mage::line<T>(T zeroAngle), name);\
MAGE_DeclareScriptingCopyOperator(mage::line<T>);\
MAGE_DeclareScriptingFunction(&mage::line<T>::endOffset, "endOffset");\
MAGE_DeclareScriptingFunction(&mage::line<T>::start, "start");\
MAGE_DeclareScriptingFunction(&mage::line<T>::orientation, "orientation");\
MAGE_DeclareScriptingFunction(&mage::line<T>::getPointDifference, "getPointDifference");\
MAGE_DeclareScriptingFunction(&mage::line<T>::getDistance, "getDistance");\
MAGE_DeclareScriptingFunction(&mage::line<T>::getAngle, "getAngle");\
MAGE_DeclareScriptingFunction(&mage::line<T>::setStartOffset, "setStartOffset");\
MAGE_DeclareScriptingFunction(&mage::line<T>::setEndPoint, "setEndPoint");\
MAGE_DeclareScriptingFunction(&mage::line<T>::getEndPoint, "getEndPoint");\
MAGE_DeclareScriptingCustom(chaiscript::fun<bool, mage::line<T>, const mage::point<T>&>(&mage::line<T>::intersects), "intersects");\
MAGE_DeclareScriptingCustom(chaiscript::fun<bool, mage::line<T>, const mage::line<T>&>(&mage::line<T>::intersects), "intersects");
