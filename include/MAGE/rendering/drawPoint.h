#pragma once

// mage::drawPoint
// -------------
// A point that can be drawn to the screen.
// Is a renderable.
// -------------
// Category: Rendering
// Has Hooks: Yes
// Script-Accessable: Yes
// -------------

#include "renderable.h"
#include "transformable.h"

namespace mage {

class MAGEDLL drawPoint : public renderable, public transformable {
public:
	drawPoint();
	drawPoint(const pointF& point);

public:
	colour tint;
	renderStates states;

protected:
	virtual renderRecipe generateDrawRecipe();
};

}