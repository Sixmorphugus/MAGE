#pragma once

// mage::transformableLine
// -------------
// A transformable line that has an end point.
// -------------
// Category: Maths
// Has Hooks: Yes
// Script-Accessable: Yes
// -------------

#include "transformable.h"
#include "line.h"

namespace mage {
class MAGEDLL transformableLine : public virtual transformable {
public:
	transformableLine();
	transformableLine(pointF position, point2F endOffset);

	lineF getLine() const; // base box, generated from the position and size of the transformable
	lineF getTransformedLine() const;

	point2F getEndOffset();
	void setEndOffset(point2F& offset);

	virtual void pixelLock();

protected:
	virtual void doTransformUpdate();

public:
	hook<transformableLine*> onEndMoved;

private:
	point2F m_endOffset;
};
}