#include "transformableLine.h"

using namespace mage;

transformableLine::transformableLine()
{
}

transformableLine::transformableLine(pointF position, point2F endOffset)
	: transformable(position)
{
	m_endOffset = endOffset;
}

lineF transformableLine::getLine() const
{
	return lineF(getRealPosition(), m_endOffset);
}

lineF transformableLine::getTransformedLine() const
{
	lineF tLine(m_transformedPoints[0].to3(), m_transformedPoints[1], CORNERS);
	tLine.start.z = getRealPosition().z;

	return tLine;
}

point2F transformableLine::getEndOffset()
{
	return m_endOffset;
}

void transformableLine::setEndOffset(point2F & offset)
{
	m_endOffset = offset;

	doTransformUpdate();
	onEndMoved.notify(this);
}

void transformableLine::pixelLock()
{
	transformable::pixelLock();
	m_endOffset.floor();
}

void transformableLine::doTransformUpdate()
{
	// a tricky one.

	point2F pos = getRealPosition().to2() + m_endOffset;

	if (getRotation() != 0) {
		// as usual; if the anchor has moved, we have to rotate around it.
		pos = pos.getRotatedAround(getRotationalCenter().to2(), getRotation());
	}

	// we can call into the base function to finish the job
	transformable::doTransformUpdate();

	// add our new point
	m_transformedPoints.push_back(pos);
}

#include "scriptingEngine.h"

MAGE_DeclareScriptingType(transformableLine);
MAGE_DeclareScriptingBaseClass(transformable, transformableLine);
MAGE_DeclareScriptingConstructor(transformableLine(), "transformableLine");
MAGE_DeclareScriptingConstructor(transformableLine(const transformableLine&), "transformableLine");
MAGE_DeclareScriptingConstructor(transformableLine(pointF, point2F), "transformableLine");
MAGE_DeclareScriptingCopyOperator(transformableLine);
MAGE_DeclareScriptingFunction(&transformableLine::getLine, "getLine");
MAGE_DeclareScriptingFunction(&transformableLine::getTransformedLine, "getTransformedLine");
MAGE_DeclareScriptingFunction(&transformableLine::setEndOffset, "setEndOffset");
MAGE_DeclareScriptingFunction(&transformableLine::getEndOffset, "getEndOffset");
MAGE_DeclareScriptingFunction(&transformableLine::onEndMoved, "onEndMoved");

MAGE_DeclareScriptingHook("transformableLineHook", transformableLine*);